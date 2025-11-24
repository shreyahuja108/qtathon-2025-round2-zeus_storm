#include "AlertLogModel.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QDebug>

AlertLogModel::AlertLogModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AlertLogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_alerts.count();
}

QVariant AlertLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_alerts.count()) {
        return QVariant();
    }

    const Alert &alert = m_alerts.at(index.row());

    switch (role) {
    case IdRole:
        return alert.id;
    case TimestampRole:
        return alert.timestamp;
    case CameraNameRole:
        return alert.cameraName;
    case TypeRole:
        return alert.type;
    case MessageRole:
        return alert.message;
    case SnapshotPathRole:
        return alert.snapshotPath;
    case HasImageRole:
        return !alert.snapshotImage.isNull();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AlertLogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TimestampRole] = "timestamp";
    roles[CameraNameRole] = "cameraName";
    roles[TypeRole] = "type";
    roles[MessageRole] = "message";
    roles[SnapshotPathRole] = "snapshotPath";
    roles[HasImageRole] = "hasImage";
    return roles;
}

void AlertLogModel::addSnapshotAlert(const QString &cameraName, const QImage &image)
{
    Alert alert;
    alert.id = generateId();
    alert.timestamp = QDateTime::currentDateTime();
    alert.cameraName = cameraName;
    alert.type = "snapshot";
    alert.message = "Snapshot captured (unsaved)";
    alert.snapshotPath = "";  // No path yet - not saved to disk
    alert.snapshotImage = image;  // Store image in memory

    addAlert(alert);
}

void AlertLogModel::addMotionAlert(const QString &cameraName, 
                                   const QString &message,
                                   const QString &snapshotPath)
{
    Alert alert;
    alert.id = generateId();
    alert.timestamp = QDateTime::currentDateTime();
    alert.cameraName = cameraName;
    alert.type = "motion";
    alert.message = message.isEmpty() ? "Motion detected" : message;
    alert.snapshotPath = snapshotPath;

    addAlert(alert);
}

void AlertLogModel::addRoiMotionAlert(const QString &cameraName,
                                       const QString &message,
                                       const QString &snapshotPath)
{
    Alert alert;
    alert.id = generateId();
    alert.timestamp = QDateTime::currentDateTime();
    alert.cameraName = cameraName;
    alert.type = "motion_roi";
    alert.message = message.isEmpty() ? "Motion in ROI" : message;
    alert.snapshotPath = snapshotPath;

    addAlert(alert);
}

void AlertLogModel::addTripwireAlert(const QString &cameraName,
                                      const QString &message,
                                      const QString &snapshotPath,
                                      int direction)
{
    Q_UNUSED(direction);
    
    Alert alert;
    alert.id = generateId();
    alert.timestamp = QDateTime::currentDateTime();
    alert.cameraName = cameraName;
    alert.type = "tripwire";
    alert.message = message.isEmpty() ? "Tripwire crossed" : message;
    alert.snapshotPath = snapshotPath;

    addAlert(alert);
}

void AlertLogModel::addLoiteringAlert(const QString &cameraName,
                                       const QString &message,
                                       const QString &snapshotPath)
{
    Alert alert;
    alert.id = generateId();
    alert.timestamp = QDateTime::currentDateTime();
    alert.cameraName = cameraName;
    alert.type = "loitering";
    alert.message = message.isEmpty() ? "Loitering detected" : message;
    alert.snapshotPath = snapshotPath;

    addAlert(alert);
}

void AlertLogModel::clear()
{
    if (m_alerts.isEmpty()) {
        return;
    }

    beginResetModel();
    m_alerts.clear();
    endResetModel();
    emit countChanged();
}

void AlertLogModel::removeAlert(int index)
{
    if (index < 0 || index >= m_alerts.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_alerts.removeAt(index);
    endRemoveRows();
    emit countChanged();
    
    qDebug() << "Alert removed at index:" << index;
}

void AlertLogModel::removeAlerts(const QVariantList &indices)
{
    if (indices.isEmpty()) {
        return;
    }

    // Convert to sorted list of integers in descending order
    QList<int> sortedIndices;
    for (const QVariant &var : indices) {
        bool ok;
        int idx = var.toInt(&ok);
        if (ok && idx >= 0 && idx < m_alerts.count()) {
            sortedIndices.append(idx);
        }
    }
    
    std::sort(sortedIndices.begin(), sortedIndices.end(), std::greater<int>());
    
    // Remove from highest to lowest
    for (int idx : sortedIndices) {
        if (idx >= 0 && idx < m_alerts.count()) {
            beginRemoveRows(QModelIndex(), idx, idx);
            m_alerts.removeAt(idx);
            endRemoveRows();
        }
    }
    
    emit countChanged();
    qDebug() << "Removed" << sortedIndices.count() << "alerts";
}

void AlertLogModel::addAlert(const Alert &alert)
{
    beginInsertRows(QModelIndex(), m_alerts.count(), m_alerts.count());
    m_alerts.append(alert);
    endInsertRows();
    emit countChanged();
    emit alertAdded(alert);
    
    qDebug() << "Alert added:" << alert.type << alert.cameraName << alert.message;
}

QString AlertLogModel::generateId() const
{
    return QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz");
}

QString AlertLogModel::getSuggestedPngFilename(int index) const
{
    if (index < 0 || index >= m_alerts.count()) {
        return "snapshot.png";
    }
    
    const Alert &alert = m_alerts.at(index);
    
    // Format: CameraName_YYYYMMDD_HHMMSS.png
    QString cameraName = alert.cameraName;
    cameraName.replace(" ", "_");  // Replace spaces with underscores
    
    QString timestamp = alert.timestamp.toString("yyyyMMdd_HHmmss");
    
    return QString("%1_%2.png").arg(cameraName).arg(timestamp);
}

bool AlertLogModel::exportSnapshotAsPng(int index, const QString &filePath)
{
    if (index < 0 || index >= m_alerts.count()) {
        qWarning() << "Invalid alert index:" << index;
        return false;
    }
    
    const Alert &alert = m_alerts.at(index);
    
    if (alert.snapshotImage.isNull()) {
        qWarning() << "No image available for alert at index:" << index;
        return false;
    }
    
    // Ensure directory exists
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create directory:" << dir.path();
            return false;
        }
    }
    
    // Save the image as PNG
    if (!alert.snapshotImage.save(filePath, "PNG")) {
        qWarning() << "Failed to save PNG to:" << filePath;
        return false;
    }
    
    qDebug() << "Snapshot exported to PNG:" << filePath;
    
    // Update the alert to mark it as saved
    Alert &mutableAlert = const_cast<Alert&>(m_alerts[index]);
    mutableAlert.snapshotPath = filePath;
    mutableAlert.message = "Snapshot saved";
    
    // Notify that this row changed
    QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex);
    
    return true;
}

bool AlertLogModel::exportToCsv(const QString &filePath)
{
    return exportAlertsToCsv(filePath, m_alerts);
}

bool AlertLogModel::exportToJson(const QString &filePath)
{
    return exportAlertsToJson(filePath, m_alerts);
}

bool AlertLogModel::exportSelectedToCsv(const QString &filePath, const QVariantList &indices)
{
    QVector<Alert> selectedAlerts;
    
    for (const QVariant &var : indices) {
        bool ok;
        int idx = var.toInt(&ok);
        if (ok && idx >= 0 && idx < m_alerts.count()) {
            selectedAlerts.append(m_alerts.at(idx));
        }
    }
    
    if (selectedAlerts.isEmpty()) {
        qWarning() << "No valid alerts to export";
        return false;
    }
    
    return exportAlertsToCsv(filePath, selectedAlerts);
}

bool AlertLogModel::exportSelectedToJson(const QString &filePath, const QVariantList &indices)
{
    QVector<Alert> selectedAlerts;
    
    for (const QVariant &var : indices) {
        bool ok;
        int idx = var.toInt(&ok);
        if (ok && idx >= 0 && idx < m_alerts.count()) {
            selectedAlerts.append(m_alerts.at(idx));
        }
    }
    
    if (selectedAlerts.isEmpty()) {
        qWarning() << "No valid alerts to export";
        return false;
    }
    
    return exportAlertsToJson(filePath, selectedAlerts);
}

bool AlertLogModel::exportAlertsToCsv(const QString &filePath, const QVector<Alert> &alerts)
{
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create directory:" << dir.path();
            return false;
        }
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    QTextStream out(&file);
    
    // Write CSV header
    out << "ID,Timestamp,Camera Name,Type,Message,Snapshot Path\n";

    // Write data rows
    for (const Alert &alert : alerts) {
        auto escape = [](const QString &field) -> QString {
            QString escaped = field;
            escaped.replace("\"", "\"\"");
            if (escaped.contains(",") || escaped.contains("\"") || escaped.contains("\n")) {
                escaped = "\"" + escaped + "\"";
            }
            return escaped;
        };

        out << escape(alert.id) << ","
            << escape(alert.timestamp.toString(Qt::ISODate)) << ","
            << escape(alert.cameraName) << ","
            << escape(alert.type) << ","
            << escape(alert.message) << ","
            << escape(alert.snapshotPath) << "\n";
    }

    file.close();
    qDebug() << "Exported" << alerts.count() << "alerts to CSV:" << filePath;
    return true;
}

bool AlertLogModel::exportAlertsToJson(const QString &filePath, const QVector<Alert> &alerts)
{
    QFileInfo fileInfo(filePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create directory:" << dir.path();
            return false;
        }
    }

    QJsonArray alertsArray;

    for (const Alert &alert : alerts) {
        QJsonObject alertObj;
        alertObj["id"] = alert.id;
        alertObj["timestamp"] = alert.timestamp.toString(Qt::ISODate);
        alertObj["cameraName"] = alert.cameraName;
        alertObj["type"] = alert.type;
        alertObj["message"] = alert.message;
        alertObj["snapshotPath"] = alert.snapshotPath;
        alertObj["hasImage"] = !alert.snapshotImage.isNull();
        
        alertsArray.append(alertObj);
    }

    QJsonObject root;
    root["alerts"] = alertsArray;
    root["exportTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    root["totalCount"] = alerts.count();

    QJsonDocument doc(root);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Exported" << alerts.count() << "alerts to JSON:" << filePath;
    return true;
}