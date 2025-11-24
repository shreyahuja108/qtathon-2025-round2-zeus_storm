#ifndef CAMERASTREAM_H
#define CAMERASTREAM_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <memory>
#include "ObjectDetector.h"

/**
 * @brief Lightweight tracking state for a single detected object
 */
struct TrackState {
    int id;                     // Unique track ID
    QString label;              // Class name (e.g., "person")
    QPointF centroid;           // Current centroid in normalized [0,1] coordinates
    QPointF prevCentroid;       // Previous centroid
    qint64 firstSeenMs;         // Timestamp when track first appeared
    qint64 lastSeenMs;          // Last update timestamp
    bool insideRoi;             // Whether last position was inside ROI
    bool loiterAlertSent;       // Flag to prevent spam
    qint64 lastTripwireAlertMs; // Last time tripwire alert was sent for this track
    qint64 enteredRoiMs;        // Timestamp when track entered ROI
    
    TrackState()
        : id(0)
        , firstSeenMs(0)
        , lastSeenMs(0)
        , insideRoi(false)
        , loiterAlertSent(false)
        , lastTripwireAlertMs(0)
        , enteredRoiMs(0)
    {}
};

/**
 * @brief Worker thread class that handles OpenCV camera capture
 */
class CaptureWorker : public QObject
{
    Q_OBJECT

public:
    explicit CaptureWorker(int cameraIndex = 0);
    ~CaptureWorker();

    void setSource(int cameraIndex);
    void setSourceUrl(const QString &url);
    void processMotionDetection(const cv::Mat &frame);
    void processRoiMotion(const cv::Mat &motionMask, int width, int height);
    void processTripwire(const cv::Mat &motionMask, int width, int height);
    void processAIDetection(const cv::Mat &frame);

public slots:
    void start();
    void stop();
    void captureFrame();
    void setMotionEnabled(bool enabled) { m_motionEnabled = enabled; }
    void setMotionSensitivity(double sensitivity) { m_motionSensitivity = sensitivity; }
    void setRoiPolygon(const QVector<QPointF> &normalizedPoints);
    void clearRoi();
    void setTripwire(const QPointF &startNorm, const QPointF &endNorm);
    void clearTripwire();
    void setAiEnabled(bool enabled);
    void setAiConfidenceThreshold(double threshold);
    void setObjectDetector(ObjectDetector *detector);

signals:
    void frameCaptured(const QImage &frame);
    void fpsUpdated(double fps);
    void errorOccurred(const QString &error);
    void motionDetected(double score, const cv::Mat &frame);
    void roiMotionDetected(double score, const cv::Mat &frame);
    void tripwireCrossed(int direction, const cv::Mat &frame);
    void aiDetectionsReady(const std::vector<Detection> &detections);
    void trackCrossedTripwire(int trackId, const QString &label, const QString &direction, const cv::Mat &frame);
    void loiteringDetected(int trackId, const QString &label, qint64 durationMs, const cv::Mat &frame);

private:
    cv::VideoCapture m_capture;
    int m_cameraIndex;
    QString m_sourceUrl;
    bool m_isUrlSource;
    std::atomic<bool> m_running;
    QTimer *m_timer;
    
    // FPS calculation
    qint64 m_lastFrameTime;
    int m_frameCount;
    double m_currentFps;
    
    // Motion detection
    cv::Ptr<cv::BackgroundSubtractorMOG2> m_backgroundSubtractor;
    bool m_motionEnabled;
    double m_motionSensitivity;
    qint64 m_lastMotionTime;
    
    // ROI & Tripwire
    QVector<QPointF> m_roiNorm;
    bool m_hasRoi;
    qint64 m_lastRoiAlertTime;
    
    QPointF m_tripwireStartNorm;
    QPointF m_tripwireEndNorm;
    bool m_hasTripwire;
    qint64 m_lastTripwireAlertTime;
    double m_prevSide;
    bool m_hasPrevSide;
    
    // AI Detection
    ObjectDetector *m_detector;
    std::atomic<bool> m_aiEnabled;  // Atomic for lock-free toggle
    int m_aiFrameCounter;
    static constexpr int AI_PROCESS_INTERVAL = 5; // Process every 5 frames
    
    // Lightweight tracking
    QMap<int, TrackState> m_tracks;       // Active tracks by ID
    int m_nextTrackId;                     // Next track ID to assign
    static constexpr double MAX_TRACK_DISTANCE = 0.1;  // Max distance in normalized coords
    static constexpr qint64 TRACK_TIMEOUT_MS = 2000;   // Remove tracks not seen for 2 seconds
    static constexpr double LINE_EPSILON = 1e-4;       // Epsilon for line crossing detection
    static constexpr qint64 TRIPWIRE_ALERT_DEBOUNCE_MS = 2000;  // 2 seconds between alerts per track
    static constexpr qint64 LOITERING_THRESHOLD_MS = 8000;      // 8 seconds to trigger loitering alert
    
    // Helper methods for tracking
    void updateTracks(const std::vector<Detection> &detections, int frameWidth, int frameHeight);
    void cleanupStaleTracks(qint64 currentTime);
    void logTracks() const;
    
    // Helper methods for line crossing
    double computeSideOfLine(const QPointF &point) const;
    QString getCrossingDirection(double prevSide, double currSide) const;
    void checkLineCrossing(TrackState &track, qint64 currentTime);
    
    // Helper methods for loitering detection
    bool pointInRoi(const QPointF &point) const;
    void updateRoiStatus(TrackState &track, qint64 currentTime);
    void checkLoitering(TrackState &track, qint64 currentTime);
};

/**
 * @brief Main camera stream class exposed to QML
 */
class CameraStream : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString source READ source CONSTANT)
    Q_PROPERTY(QString sourceType READ sourceType CONSTANT)
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(double fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(bool motionEnabled READ motionEnabled WRITE setMotionEnabled NOTIFY motionEnabledChanged)
    Q_PROPERTY(double motionSensitivity READ motionSensitivity WRITE setMotionSensitivity NOTIFY motionSensitivityChanged)
    Q_PROPERTY(bool motionActive READ motionActive NOTIFY motionActiveChanged)
    Q_PROPERTY(bool roiAlertActive READ roiAlertActive NOTIFY roiAlertActiveChanged)
    Q_PROPERTY(bool tripwireAlertActive READ tripwireAlertActive NOTIFY tripwireAlertActiveChanged)
    Q_PROPERTY(bool aiEnabled READ aiEnabled WRITE setAiEnabled NOTIFY aiEnabledChanged)
    Q_PROPERTY(double aiConfidenceThreshold READ aiConfidenceThreshold WRITE setAiConfidenceThreshold NOTIFY aiConfidenceThresholdChanged)
    Q_PROPERTY(QVariantList detections READ detections NOTIFY detectionsChanged)
Q_PROPERTY(bool autoSnapshotOnMotion READ autoSnapshotOnMotion WRITE setAutoSnapshotOnMotion NOTIFY autoSnapshotOnMotionChanged)
Q_PROPERTY(bool autoSnapshotOnRoi READ autoSnapshotOnRoi WRITE setAutoSnapshotOnRoi NOTIFY autoSnapshotOnRoiChanged)
Q_PROPERTY(bool autoSnapshotOnTripwire READ autoSnapshotOnTripwire WRITE setAutoSnapshotOnTripwire NOTIFY autoSnapshotOnTripwireChanged)

public:
    explicit CameraStream(const QString &id, const QString &source, const QString &sourceType, const QString &name, QObject *parent = nullptr);
    ~CameraStream();

    // Property getters
    QString id() const { return m_id; }
    QString source() const { return m_source; }
    QString sourceType() const { return m_sourceType; }
    QImage frame() const { return m_currentFrame; }
    bool isRunning() const { return m_running; }
    double fps() const { return m_fps; }
    QString status() const { return m_status; }
    QString cameraName() const { return m_cameraName; }
    bool motionEnabled() const { return m_motionEnabled; }
    double motionSensitivity() const { return m_motionSensitivity; }
    bool motionActive() const { return m_motionActive; }
    bool roiAlertActive() const { return m_roiAlertActive; }
    bool tripwireAlertActive() const { return m_tripwireAlertActive; }
    bool aiEnabled() const { return m_aiEnabled; }
    double aiConfidenceThreshold() const { return m_aiConfidenceThreshold; }
    QVariantList detections() const;
    
bool autoSnapshotOnMotion() const { return m_autoSnapshotOnMotion; }
void setAutoSnapshotOnMotion(bool enabled);

bool autoSnapshotOnRoi() const { return m_autoSnapshotOnRoi; }
void setAutoSnapshotOnRoi(bool enabled);

bool autoSnapshotOnTripwire() const { return m_autoSnapshotOnTripwire; }
void setAutoSnapshotOnTripwire(bool enabled);

    void setCameraName(const QString &name) { m_cameraName = name; emit cameraNameChanged(); }
    void setMotionEnabled(bool enabled);
    void setMotionSensitivity(double sensitivity);
    void setAiEnabled(bool enabled);
    void setAiConfidenceThreshold(double threshold);
    void setObjectDetector(ObjectDetector *detector);

    // Invokable methods for QML
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setSource(int cameraIndex);
    Q_INVOKABLE bool takeSnapshot();  // NEW: Just captures, doesn't save
    Q_INVOKABLE bool saveSnapshot(const QString &targetDir);
    
    // Additional configuration methods
    void setSourceDevice(int deviceIndex);
    void setSourceUrl(const QString &url);
    void setRoiPolygon(const QVector<QPointF> &normalizedPoints);
    void clearRoi();
    void setTripwire(const QPointF &startNorm, const QPointF &endNorm);
    void clearTripwire();

signals:
    void frameChanged();
    void runningChanged();
    void fpsChanged();
    void statusChanged();
    void cameraNameChanged();
    void snapshotSaved(const QString &filePath);
    void snapshotFailed(const QString &reason);
    void motionEnabledChanged();
    void motionSensitivityChanged();
    void motionActiveChanged();
    void motionDetected(double score);
    void roiMotionDetected(double score);
    void tripwireCrossed(int direction);
    void trackCrossedTripwire(int trackId, const QString &label, const QString &direction);
    void loiteringDetected(int trackId, const QString &label, qint64 durationMs);
    void roiAlertActiveChanged();
    void tripwireAlertActiveChanged();
    void aiEnabledChanged();
    void aiConfidenceThresholdChanged();
    void detectionsChanged();
     void snapshotCaptured(const QImage &image);  // NEW: Emits captured frame
void autoSnapshotOnMotionChanged();
    void autoSnapshotOnRoiChanged();
    void autoSnapshotOnTripwireChanged();


private slots:
    void onFrameCaptured(const QImage &frame);
    void onFpsUpdated(double fps);
    void onErrorOccurred(const QString &error);
    void onMotionDetected(double score, const cv::Mat &frame);
    void onRoiMotionDetected(double score, const cv::Mat &frame);
    void onTripwireCrossed(int direction, const cv::Mat &frame);
    void onTrackCrossedTripwire(int trackId, const QString &label, const QString &direction, const cv::Mat &frame);
    void onLoiteringDetected(int trackId, const QString &label, qint64 durationMs, const cv::Mat &frame);
    void onAIDetectionsReady(const std::vector<Detection> &detections);
    void resetMotionActive();
    void resetRoiAlertActive();
    void resetTripwireAlertActive();

private:
    QString m_id;
    QString m_source;
    QString m_sourceType;
    QImage m_currentFrame;
    bool m_running;
    double m_fps;
    QString m_status;
    QString m_cameraName;
    int m_cameraIndex;
    QString m_sourceUrl;
    bool m_isUrlSource;
    bool m_autoSnapshotOnMotion;
    bool m_autoSnapshotOnRoi;
    bool m_autoSnapshotOnTripwire;

    // Motion detection
    bool m_motionEnabled;
    double m_motionSensitivity;
    bool m_motionActive;
    QTimer *m_motionResetTimer;
    qint64 m_lastMotionAlertTime;
    
    // ROI & Tripwire
    QVector<QPointF> m_roiNorm;
    bool m_hasRoi;
    bool m_roiAlertActive;
    QTimer *m_roiAlertResetTimer;
    qint64 m_lastRoiAlertTime;
    
    QPointF m_tripwireStartNorm;
    QPointF m_tripwireEndNorm;
    bool m_hasTripwire;
    bool m_tripwireAlertActive;
    QTimer *m_tripwireAlertResetTimer;
    qint64 m_lastTripwireAlertTime;
    double m_prevSide;
    bool m_hasPrevSide;
    
    // AI Detection
    ObjectDetector *m_detector;
    bool m_aiEnabled;
    double m_aiConfidenceThreshold;
    std::vector<Detection> m_currentDetections;
    
    QThread *m_workerThread;
    CaptureWorker *m_worker;
    
    mutable QMutex m_frameMutex;
    mutable QMutex m_detectionMutex;
};

#endif // CAMERASTREAM_H