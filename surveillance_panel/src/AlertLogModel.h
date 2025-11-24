#ifndef ALERTLOGMODEL_H
#define ALERTLOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <QString>
#include <QVector>
#include <QImage>

/**
 * @brief Structure representing a single alert entry
 */
struct Alert {
    QString id;
    QDateTime timestamp;
    QString cameraName;
    QString type;           // "snapshot", "motion", etc.
    QString message;
    QString snapshotPath;   // Optional, for snapshot alerts
    QImage snapshotImage;   // NEW: In-memory image for unsaved snapshots
};

/**
 * @brief Model for managing and displaying alert log entries
 */
class AlertLogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum AlertRoles {
        IdRole = Qt::UserRole + 1,
        TimestampRole,
        CameraNameRole,
        TypeRole,
        MessageRole,
        SnapshotPathRole,
        HasImageRole        // NEW: Indicates if alert has an in-memory image
    };

    explicit AlertLogModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Alert management
    Q_INVOKABLE void addSnapshotAlert(const QString &cameraName, const QImage &image);
    Q_INVOKABLE void addMotionAlert(const QString &cameraName, 
                                     const QString &message = QString("Motion detected"),
                                     const QString &snapshotPath = QString());
    Q_INVOKABLE void addRoiMotionAlert(const QString &cameraName,
                                        const QString &message,
                                        const QString &snapshotPath = QString());
    Q_INVOKABLE void addTripwireAlert(const QString &cameraName,
                                       const QString &message,
                                       const QString &snapshotPath = QString(),
                                       int direction = 0);
    Q_INVOKABLE void addLoiteringAlert(const QString &cameraName,
                                        const QString &message,
                                        const QString &snapshotPath = QString());
    Q_INVOKABLE void clear();
    
    // Export functions
    Q_INVOKABLE bool exportToCsv(const QString &filePath);
    Q_INVOKABLE bool exportToJson(const QString &filePath);
    Q_INVOKABLE bool exportSelectedToCsv(const QString &filePath, const QVariantList &indices);
    Q_INVOKABLE bool exportSelectedToJson(const QString &filePath, const QVariantList &indices);
    Q_INVOKABLE bool exportSnapshotAsPng(int index, const QString &filePath);  // NEW
    
    // Removal functions
    Q_INVOKABLE void removeAlerts(const QVariantList &indices);
    Q_INVOKABLE void removeAlert(int index);
    
    // Helper to get suggested filename for PNG export
    Q_INVOKABLE QString getSuggestedPngFilename(int index) const;

signals:
    void countChanged();
    void alertAdded(const Alert &alert);

private:
    void addAlert(const Alert &alert);
    QString generateId() const;
    bool exportAlertsToCsv(const QString &filePath, const QVector<Alert> &alerts);
    bool exportAlertsToJson(const QString &filePath, const QVector<Alert> &alerts);

    QVector<Alert> m_alerts;
};

#endif // ALERTLOGMODEL_H