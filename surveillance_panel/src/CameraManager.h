#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QObject>
#include <QString>
#include <QVector>
#include <memory>
#include "CameraStream.h"
#include "ObjectDetector.h"

// Forward declaration
class CameraImageProvider;

/**
 * @brief Configuration structure for a single camera
 */
struct CameraConfig {
    QString id;          // e.g., "cam1"
    QString name;        // Display name
    QString type;        // "usb", "rtsp", "ip"
    QString source;      // device index or URL
    bool enabled;
    QVector<QPointF> roiPoints;      // Normalized 0-1 coordinates
    QPointF tripwireStart;            // Normalized 0-1
    QPointF tripwireEnd;              // Normalized 0-1
    bool hasRoi;
    bool hasTripwire;
};

/**
 * @brief Manages multiple camera streams and their configuration
 */
class CameraManager : public QObject
{
    Q_OBJECT
    
    // Expose individual camera streams as properties
    Q_PROPERTY(QObject* camera1 READ camera1 CONSTANT)
    Q_PROPERTY(QObject* camera2 READ camera2 CONSTANT)
    Q_PROPERTY(QObject* camera3 READ camera3 CONSTANT)
    Q_PROPERTY(QObject* camera4 READ camera4 CONSTANT)

public:
    explicit CameraManager(QObject *parent = nullptr);
    ~CameraManager();

    // Property getters
    QObject* camera1() const { return m_cameras.value(0, nullptr); }
    QObject* camera2() const { return m_cameras.value(1, nullptr); }
    QObject* camera3() const { return m_cameras.value(2, nullptr); }
    QObject* camera4() const { return m_cameras.value(3, nullptr); }

    // Invokable methods for QML
    Q_INVOKABLE QString cameraName(int index) const;
    Q_INVOKABLE bool cameraAvailable(int index) const;
    Q_INVOKABLE QString cameraType(int index) const;
    Q_INVOKABLE QString cameraSource(int index) const;
    
    // ROI methods
    Q_INVOKABLE QVariantList roiPoints(int index) const;
    Q_INVOKABLE bool hasRoi(int index) const;
    Q_INVOKABLE void setRoi(int index, const QVariantList &points);
    Q_INVOKABLE void clearRoi(int index);
    
    // Tripwire methods
    Q_INVOKABLE QVariantMap tripwire(int index) const;
    Q_INVOKABLE bool hasTripwire(int index) const;
    Q_INVOKABLE void setTripwire(int index, double startX, double startY, double endX, double endY);
    Q_INVOKABLE void clearTripwire(int index);

signals:
    void roiChanged(int index);
    void tripwireChanged(int index);

private:
    bool loadConfiguration(const QString &configPath);
    void createCameraStreams();
    bool saveConfiguration();

    QString m_configPath;
    QVector<CameraConfig> m_configs;
    QVector<CameraStream*> m_cameras;  // Up to 4 cameras
    std::unique_ptr<ObjectDetector> m_detector;
};

#endif // CAMERAMANAGER_H