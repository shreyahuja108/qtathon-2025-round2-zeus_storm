#include "CameraManager.h"
#include "CameraImageProvider.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <algorithm>

CameraManager::CameraManager(QObject *parent)
    : QObject(parent)
{
    // Construct path to config file
    QString appDir = QCoreApplication::applicationDirPath();
    m_configPath = QDir(appDir).filePath("cameras.json");  // Right next to exe
    
    qDebug() << "Loading camera configuration from:" << m_configPath;
    
    // Initialize ObjectDetector
    QString modelPath = QDir(appDir).filePath("../assets/models/yolov8n.onnx");
    QString classNamesPath = QDir(appDir).filePath("../assets/models/coco.names");
    
    try {
        m_detector = std::make_unique<ObjectDetector>(
            modelPath.toStdString(),
            classNamesPath.toStdString(),
            0.5f,  // default confidence threshold
            0.45f  // NMS threshold
        );
        
        if (m_detector->isLoaded()) {
            qDebug() << "ObjectDetector initialized successfully";
        } else {
            qWarning() << "Warning: ObjectDetector failed to load model or class names";
        }
    } catch (const std::exception &e) {
        qWarning() << "Error creating ObjectDetector:" << e.what();
    }

    // Load configuration
    if (!loadConfiguration(m_configPath)) {
        qWarning() << "Failed to load camera configuration, using defaults";
        
        // Create a default single camera if config loading fails
        CameraConfig defaultConfig;
        defaultConfig.id = "cam1";  // String ID
        defaultConfig.name = "Default Camera";
        defaultConfig.type = "usb";
        defaultConfig.source = "0";
        defaultConfig.enabled = true;
        defaultConfig.hasRoi = false;
        defaultConfig.hasTripwire = false;
        m_configs.append(defaultConfig);
    }

    // Create camera streams based on configuration
    createCameraStreams();
}

CameraManager::~CameraManager()
{
    // Clean up camera streams
    qDeleteAll(m_cameras);
    m_cameras.clear();
}

bool CameraManager::loadConfiguration(const QString &configPath)
{
    QFile file(configPath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open config file:" << configPath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Invalid JSON in config file";
        return false;
    }

    QJsonObject root = doc.object();
    QJsonArray camerasArray = root["cameras"].toArray();

    m_configs.clear();

    for (const QJsonValue &value : camerasArray) {
        QJsonObject camObj = value.toObject();
        
        CameraConfig config;
        config.id = camObj["id"].toString();  // Now string
        config.name = camObj["name"].toString();
        config.type = camObj["type"].toString();
        config.source = camObj["source"].toVariant().toString();
        config.enabled = camObj["enabled"].toBool();
        
        // Parse ROI if present
        config.hasRoi = false;
        if (camObj.contains("roi")) {
            QJsonObject roiObj = camObj["roi"].toObject();
            if (roiObj.contains("points")) {
                QJsonArray pointsArray = roiObj["points"].toArray();
                for (const QJsonValue &pointVal : pointsArray) {
                    QJsonObject pointObj = pointVal.toObject();
                    double x = pointObj["x"].toDouble();
                    double y = pointObj["y"].toDouble();
                    config.roiPoints.append(QPointF(x, y));
                }
                config.hasRoi = !config.roiPoints.isEmpty();
            }
        }
        
        // Parse Tripwire if present
        config.hasTripwire = false;
        if (camObj.contains("tripwire")) {
            QJsonObject tripObj = camObj["tripwire"].toObject();
            if (tripObj.contains("start") && tripObj.contains("end")) {
                QJsonObject startObj = tripObj["start"].toObject();
                QJsonObject endObj = tripObj["end"].toObject();
                
                config.tripwireStart = QPointF(startObj["x"].toDouble(), 
                                                startObj["y"].toDouble());
                config.tripwireEnd = QPointF(endObj["x"].toDouble(), 
                                              endObj["y"].toDouble());
                
                // Check if tripwire has actual coordinates (not 0,0 to 0,0)
                config.hasTripwire = !(config.tripwireStart.isNull() && config.tripwireEnd.isNull()) &&
                                     !(qFuzzyIsNull(config.tripwireStart.x()) && 
                                       qFuzzyIsNull(config.tripwireStart.y()) &&
                                       qFuzzyIsNull(config.tripwireEnd.x()) && 
                                       qFuzzyIsNull(config.tripwireEnd.y()));
            }
        }

        // Add ALL cameras to maintain correct indexing (disabled ones too)
        m_configs.append(config);
        qDebug() << "Loaded camera config" << m_configs.size()-1 << ":" 
                 << config.id << config.name << config.type << config.source 
                 << "enabled:" << config.enabled;
        
        // Stop after 4 cameras
        if (m_configs.size() >= 4) break;
    }

    qDebug() << "Total cameras in config:" << m_configs.size();
    return !m_configs.isEmpty();
}

void CameraManager::createCameraStreams()
{
    // Clear existing cameras
    qDeleteAll(m_cameras);
    m_cameras.clear();

    // Always create exactly 4 slots
    // Process configs but maintain order - cam1 always slot 0, cam2 slot 1, etc.
    for (int i = 0; i < 4; i++) {
        CameraStream *stream = nullptr;
        
        // Find config for this slot (if exists and enabled)
        if (i < m_configs.size()) {
            const CameraConfig &config = m_configs[i];
            
            if (config.enabled) {
                // Create camera stream with new constructor (id, source, sourceType, name)
                stream = new CameraStream(
                    config.id,
                    config.source,
                    config.type,
                    config.name,
                    this
                );
                
                // Set ROI if available
                if (config.hasRoi && !config.roiPoints.isEmpty()) {
                    stream->setRoiPolygon(config.roiPoints);
                }
                
                // Set Tripwire if available
                if (config.hasTripwire) {
                    stream->setTripwire(config.tripwireStart, config.tripwireEnd);
                }
                
                // Set ObjectDetector
                if (m_detector) {
                    stream->setObjectDetector(m_detector.get());
                }
            }
        }
        
        m_cameras.append(stream);
    }
    
    int enabledCount = std::count_if(m_cameras.begin(), m_cameras.end(), 
                                      [](CameraStream* s) { return s != nullptr; });
    qDebug() << "Created 4 camera slots," << enabledCount << "enabled";
    
    // Debug: show what's in each slot
    for (int i = 0; i < m_cameras.size(); i++) {
        if (m_cameras[i]) {
            qDebug() << "  Slot" << i << ":" << m_cameras[i]->cameraName() << "ENABLED";
        } else {
            qDebug() << "  Slot" << i << ": nullptr (disabled)";
        }
    }
}

QString CameraManager::cameraName(int index) const
{
    int idx = index - 1; // Convert 1-based to 0-based
    
    if (idx >= 0 && idx < m_configs.size()) {
        return m_configs[idx].name;
    }
    
    return QString("Camera %1").arg(index);
}

bool CameraManager::cameraAvailable(int index) const
{
    int idx = index - 1; // Convert 1-based to 0-based
    
    return (idx >= 0 && idx < m_cameras.size() && m_cameras[idx] != nullptr);
}

QString CameraManager::cameraType(int index) const
{
    int idx = index - 1; // Convert 1-based to 0-based
    
    if (idx >= 0 && idx < m_configs.size()) {
        return m_configs[idx].type;
    }
    
    return "unknown";
}

QString CameraManager::cameraSource(int index) const
{
    int idx = index - 1; // Convert 1-based to 0-based
    
    if (idx >= 0 && idx < m_configs.size()) {
        return m_configs[idx].source;
    }
    
    return "";
}

QVariantList CameraManager::roiPoints(int index) const
{
    int idx = index - 1;
    
    if (idx >= 0 && idx < m_configs.size()) {
        QVariantList result;
        for (const QPointF &point : m_configs[idx].roiPoints) {
            QVariantMap pointMap;
            pointMap["x"] = point.x();
            pointMap["y"] = point.y();
            result.append(pointMap);
        }
        return result;
    }
    
    return QVariantList();
}

bool CameraManager::hasRoi(int index) const
{
    int idx = index - 1;
    
    if (idx >= 0 && idx < m_configs.size()) {
        return m_configs[idx].hasRoi;
    }
    
    return false;
}

void CameraManager::setRoi(int index, const QVariantList &points)
{
    int idx = index - 1;
    
    if (idx < 0 || idx >= m_configs.size()) {
        return;
    }
    
    m_configs[idx].roiPoints.clear();
    
    for (const QVariant &pointVar : points) {
        QVariantMap pointMap = pointVar.toMap();
        double x = pointMap["x"].toDouble();
        double y = pointMap["y"].toDouble();
        m_configs[idx].roiPoints.append(QPointF(x, y));
    }
    
    m_configs[idx].hasRoi = !m_configs[idx].roiPoints.isEmpty();
    
    // Update the CameraStream
    if (idx < m_cameras.size() && m_cameras[idx] != nullptr) {
        m_cameras[idx]->setRoiPolygon(m_configs[idx].roiPoints);
    }
    
    qDebug() << "ROI set for camera" << index << "with" << m_configs[idx].roiPoints.size() << "points";
    
    saveConfiguration();
    emit roiChanged(index);
}

void CameraManager::clearRoi(int index)
{
    int idx = index - 1;
    
    if (idx < 0 || idx >= m_configs.size()) {
        return;
    }
    
    m_configs[idx].roiPoints.clear();
    m_configs[idx].hasRoi = false;
    
    // Update the CameraStream
    if (idx < m_cameras.size() && m_cameras[idx] != nullptr) {
        m_cameras[idx]->clearRoi();
    }
    
    qDebug() << "ROI cleared for camera" << index;
    
    saveConfiguration();
    emit roiChanged(index);
}

QVariantMap CameraManager::tripwire(int index) const
{
    int idx = index - 1;
    
    QVariantMap result;
    result["has"] = false;
    
    if (idx >= 0 && idx < m_configs.size() && m_configs[idx].hasTripwire) {
        result["has"] = true;
        result["startX"] = m_configs[idx].tripwireStart.x();
        result["startY"] = m_configs[idx].tripwireStart.y();
        result["endX"] = m_configs[idx].tripwireEnd.x();
        result["endY"] = m_configs[idx].tripwireEnd.y();
    }
    
    return result;
}

bool CameraManager::hasTripwire(int index) const
{
    int idx = index - 1;
    
    if (idx >= 0 && idx < m_configs.size()) {
        return m_configs[idx].hasTripwire;
    }
    
    return false;
}

void CameraManager::setTripwire(int index, double startX, double startY, double endX, double endY)
{
    int idx = index - 1;
    
    if (idx < 0 || idx >= m_configs.size()) {
        return;
    }
    
    m_configs[idx].tripwireStart = QPointF(startX, startY);
    m_configs[idx].tripwireEnd = QPointF(endX, endY);
    m_configs[idx].hasTripwire = true;
    
    // Update the CameraStream
    if (idx < m_cameras.size() && m_cameras[idx] != nullptr) {
        m_cameras[idx]->setTripwire(m_configs[idx].tripwireStart, m_configs[idx].tripwireEnd);
    }
    
    qDebug() << "Tripwire set for camera" << index 
             << "from (" << startX << "," << startY << ") to (" << endX << "," << endY << ")";
    
    saveConfiguration();
    emit tripwireChanged(index);
}

void CameraManager::clearTripwire(int index)
{
    int idx = index - 1;
    
    if (idx < 0 || idx >= m_configs.size()) {
        return;
    }
    
    m_configs[idx].tripwireStart = QPointF();
    m_configs[idx].tripwireEnd = QPointF();
    m_configs[idx].hasTripwire = false;
    
    // Update the CameraStream
    if (idx < m_cameras.size() && m_cameras[idx] != nullptr) {
        m_cameras[idx]->clearTripwire();
    }
    
    qDebug() << "Tripwire cleared for camera" << index;
    
    saveConfiguration();
    emit tripwireChanged(index);
}

bool CameraManager::saveConfiguration()
{
    QFile file(m_configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open config file for writing:" << m_configPath;
        return false;
    }
    
    QJsonObject root;
    QJsonArray camerasArray;
    
    for (const CameraConfig &config : m_configs) {
        QJsonObject camObj;
        camObj["id"] = config.id;
        camObj["name"] = config.name;
        camObj["type"] = config.type;
        
        // Handle source - could be int or string
        bool isInt;
        int sourceInt = config.source.toInt(&isInt);
        if (isInt) {
            camObj["source"] = QString::number(sourceInt);
        } else {
            camObj["source"] = config.source;
        }
        
        camObj["enabled"] = config.enabled;
        
        // Save ROI
        QJsonObject roiObj;
        QJsonArray pointsArray;
        for (const QPointF &point : config.roiPoints) {
            QJsonObject pointObj;
            pointObj["x"] = point.x();
            pointObj["y"] = point.y();
            pointsArray.append(pointObj);
        }
        roiObj["points"] = pointsArray;
        camObj["roi"] = roiObj;
        
        // Save Tripwire
        QJsonObject tripObj;
        QJsonObject startObj;
        startObj["x"] = config.tripwireStart.x();
        startObj["y"] = config.tripwireStart.y();
        tripObj["start"] = startObj;
        
        QJsonObject endObj;
        endObj["x"] = config.tripwireEnd.x();
        endObj["y"] = config.tripwireEnd.y();
        tripObj["end"] = endObj;
        camObj["tripwire"] = tripObj;
        
        camerasArray.append(camObj);
    }
    
    root["cameras"] = camerasArray;
    
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Configuration saved to:" << m_configPath;
    return true;
}