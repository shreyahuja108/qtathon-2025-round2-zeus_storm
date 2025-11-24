#include "CameraStream.h"
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>

// ============================================================================
// CaptureWorker Implementation
// ============================================================================

CaptureWorker::CaptureWorker(int cameraIndex)
    : m_cameraIndex(cameraIndex)
    , m_isUrlSource(false)
    , m_running(false)
    , m_timer(nullptr)
    , m_lastFrameTime(0)
    , m_frameCount(0)
    , m_currentFps(0.0)
    , m_motionEnabled(false)
    , m_motionSensitivity(50.0)
    , m_lastMotionTime(0)
    , m_hasRoi(false)
    , m_lastRoiAlertTime(0)
    , m_hasTripwire(false)
    , m_lastTripwireAlertTime(0)
    , m_prevSide(0.0)
    , m_hasPrevSide(false)
    , m_detector(nullptr)
    , m_aiEnabled(false)
    , m_aiFrameCounter(0)
    , m_nextTrackId(1)
{
    // Create background subtractor for motion detection
    m_backgroundSubtractor = cv::createBackgroundSubtractorMOG2(500, 16, false);
}

CaptureWorker::~CaptureWorker()
{
    stop();
}

void CaptureWorker::setSource(int cameraIndex)
{
    m_cameraIndex = cameraIndex;
    m_isUrlSource = false;
    m_sourceUrl.clear();
}

void CaptureWorker::setSourceUrl(const QString &url)
{
    m_sourceUrl = url;
    m_isUrlSource = true;
    m_cameraIndex = -1;
}

void CaptureWorker::start()
{
    if (m_running) {
        return;
    }

    // Open the camera based on source type
    if (m_isUrlSource) {
        m_capture.open(m_sourceUrl.toStdString());
    } else {
        m_capture.open(m_cameraIndex);
    }
    
    if (!m_capture.isOpened()) {
        QString source = m_isUrlSource ? m_sourceUrl : QString::number(m_cameraIndex);
        emit errorOccurred(QString("Failed to open camera: %1").arg(source));
        return;
    }

    // Set camera properties for better performance
    m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    m_capture.set(cv::CAP_PROP_FPS, 30);

    m_running = true;
    m_lastFrameTime = QDateTime::currentMSecsSinceEpoch();
    m_frameCount = 0;

    // Create timer for frame capture (30 FPS = ~33ms interval)
    if (!m_timer) {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &CaptureWorker::captureFrame);
    }
    
    m_timer->start(33); // ~30 FPS
}

void CaptureWorker::stop()
{
    if (!m_running) {
        return;
    }

    m_running = false;

    if (m_timer) {
        m_timer->stop();
    }

    if (m_capture.isOpened()) {
        m_capture.release();
    }
}

void CaptureWorker::captureFrame()
{
    if (!m_running || !m_capture.isOpened()) {
        return;
    }

    cv::Mat frame;
    m_capture >> frame;

    if (frame.empty()) {
        emit errorOccurred("Failed to capture frame");
        return;
    }

    // Process motion detection if enabled
    if (m_motionEnabled) {
        processMotionDetection(frame);
    }
    
    // Process AI detection if enabled (every N frames)
    if (m_aiEnabled.load(std::memory_order_relaxed) && m_detector && m_detector->isLoaded()) {
        m_aiFrameCounter++;
        if (m_aiFrameCounter >= AI_PROCESS_INTERVAL) {
            m_aiFrameCounter = 0;
            processAIDetection(frame);
        }
    }

    // Convert BGR to RGB
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

    // Convert cv::Mat to QImage
    QImage qImg(rgbFrame.data, 
                rgbFrame.cols, 
                rgbFrame.rows, 
                static_cast<int>(rgbFrame.step), 
                QImage::Format_RGB888);
    
    // Deep copy to ensure data persists after cv::Mat is destroyed
    QImage imageCopy = qImg.copy();

    emit frameCaptured(imageCopy);

    // Calculate FPS
    m_frameCount++;
    if (m_frameCount >= 10) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        qint64 elapsed = currentTime - m_lastFrameTime;
        
        if (elapsed > 0) {
            m_currentFps = (m_frameCount * 1000.0) / elapsed;
            emit fpsUpdated(m_currentFps);
        }
        
        m_frameCount = 0;
        m_lastFrameTime = currentTime;
    }
}

void CaptureWorker::processAIDetection(const cv::Mat &frame)
{
    if (!m_detector || !m_detector->isLoaded()) {
        return;
    }
    
    try {
        // Run inference
        std::vector<Detection> detections = m_detector->infer(frame);
        
        // Update tracks with new detections
        updateTracks(detections, frame.cols, frame.rows);
        
        // Emit detections to main thread
        emit aiDetectionsReady(detections);
        
    } catch (const std::exception &e) {
        qWarning() << "AI detection error:" << e.what();
    }
}

void CaptureWorker::setAiEnabled(bool enabled)
{
    m_aiEnabled.store(enabled, std::memory_order_relaxed);
    m_aiFrameCounter = 0;
    qDebug() << "AI detection worker state:" << (enabled ? "enabled" : "disabled");
}

void CaptureWorker::setAiConfidenceThreshold(double threshold)
{
    if (m_detector) {
        m_detector->setConfidenceThreshold(static_cast<float>(threshold));
    }
}

void CaptureWorker::setObjectDetector(ObjectDetector *detector)
{
    m_detector = detector;
}

void CaptureWorker::processMotionDetection(const cv::Mat &frame)
{
    // Apply background subtractor
    cv::Mat fgMask;
    m_backgroundSubtractor->apply(frame, fgMask);
    
    // Apply morphological operations to reduce noise
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(fgMask, fgMask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(fgMask, fgMask, cv::MORPH_CLOSE, kernel);
    
    // Count non-zero pixels (motion pixels)
    int motionPixels = cv::countNonZero(fgMask);
    int totalPixels = fgMask.rows * fgMask.cols;
    
    // Calculate motion score as percentage
    double motionScore = (motionPixels * 100.0) / totalPixels;
    
    // Map sensitivity (0-100) to threshold (high to low)
    double threshold = 10.0 - (m_motionSensitivity / 100.0) * 9.5;
    
    // Check if motion exceeds threshold
    if (motionScore > threshold) {
        // Rate limiting: minimum 2 seconds between motion events
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - m_lastMotionTime > 2000) {
            m_lastMotionTime = currentTime;
            emit motionDetected(motionScore, frame.clone());
        }
    }
    
    // Process ROI-based motion if ROI is defined
    if (m_hasRoi) {
        processRoiMotion(fgMask, frame.cols, frame.rows);
    }
    
    // Process tripwire if tripwire is defined
    if (m_hasTripwire) {
        processTripwire(fgMask, frame.cols, frame.rows);
    }
}

void CaptureWorker::processRoiMotion(const cv::Mat &motionMask, int width, int height)
{
    // Convert normalized ROI points to pixel coordinates
    std::vector<cv::Point> roiPts;
    for (const QPointF &p : m_roiNorm) {
        int x = static_cast<int>(p.x() * width);
        int y = static_cast<int>(p.y() * height);
        roiPts.push_back(cv::Point(x, y));
    }
    
    if (roiPts.size() < 3) {
        return; // Need at least 3 points for a valid polygon
    }
    
    // Create ROI mask
    cv::Mat roiMask = cv::Mat::zeros(motionMask.size(), CV_8UC1);
    std::vector<std::vector<cv::Point>> pts = { roiPts };
    cv::fillPoly(roiMask, pts, cv::Scalar(255));
    
    // Combine motion mask with ROI mask
    cv::Mat roiMotion;
    cv::bitwise_and(motionMask, roiMask, roiMotion);
    
    // Calculate ROI area for normalization
    int roiArea = cv::countNonZero(roiMask);
    int roiMotionPixels = cv::countNonZero(roiMotion);
    
    if (roiArea == 0) return;
    
    // Calculate score as percentage of ROI area
    double roiScore = (roiMotionPixels * 100.0) / roiArea;
    
    // Use similar threshold as general motion detection
    double threshold = 10.0 - (m_motionSensitivity / 100.0) * 9.5;
    
    if (roiScore > threshold) {
        // Rate limiting: minimum 3 seconds between ROI alerts
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - m_lastRoiAlertTime > 3000) {
            m_lastRoiAlertTime = currentTime;
            
            // We need the original frame for snapshot, but we only have the mask here
            // We'll pass an empty Mat and let the main thread handle snapshot
            emit roiMotionDetected(roiScore, cv::Mat());
        }
    }
}

void CaptureWorker::processTripwire(const cv::Mat &motionMask, int width, int height)
{
    // Compute centroid of motion
    cv::Moments m = cv::moments(motionMask, true);
    
    if (m.m00 < 100) {
        // Too little motion to track
        m_hasPrevSide = false;
        return;
    }
    
    double cx = m.m10 / m.m00;
    double cy = m.m01 / m.m00;
    
    // Convert tripwire to pixel coordinates
    double x1 = m_tripwireStartNorm.x() * width;
    double y1 = m_tripwireStartNorm.y() * height;
    double x2 = m_tripwireEndNorm.x() * width;
    double y2 = m_tripwireEndNorm.y() * height;
    
    // Calculate which side of the line the centroid is on
    double curSide = (cx - x1) * (y2 - y1) - (cy - y1) * (x2 - x1);
    
    // Check for line crossing
    if (m_hasPrevSide && curSide * m_prevSide < 0) {
        // Crossing detected - check if centroid is close enough to the line
        double lineLength = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        double distance = std::abs(curSide) / lineLength;
        
        // Only count as crossing if within reasonable distance (e.g., 50 pixels)
        if (distance < 50) {
            // Rate limiting: minimum 2 seconds between tripwire alerts
            qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
            if (currentTime - m_lastTripwireAlertTime > 2000) {
                m_lastTripwireAlertTime = currentTime;
                
                // Determine direction
                int direction = (curSide > 0 && m_prevSide < 0) ? 1 : -1;
                
                emit tripwireCrossed(direction, cv::Mat());
            }
        }
    }
    
    m_prevSide = curSide;
    m_hasPrevSide = true;
}

void CaptureWorker::setRoiPolygon(const QVector<QPointF> &normalizedPoints)
{
    m_roiNorm = normalizedPoints;
    m_hasRoi = !m_roiNorm.isEmpty();
    m_lastRoiAlertTime = 0;
}

void CaptureWorker::clearRoi()
{
    m_roiNorm.clear();
    m_hasRoi = false;
    m_lastRoiAlertTime = 0;
}

void CaptureWorker::setTripwire(const QPointF &startNorm, const QPointF &endNorm)
{
    m_tripwireStartNorm = startNorm;
    m_tripwireEndNorm = endNorm;
    m_hasTripwire = true;
    m_lastTripwireAlertTime = 0;
    m_hasPrevSide = false;
}

void CaptureWorker::clearTripwire()
{
    m_tripwireStartNorm = QPointF();
    m_tripwireEndNorm = QPointF();
    m_hasTripwire = false;
    m_lastTripwireAlertTime = 0;
    m_hasPrevSide = false;
}

void CaptureWorker::updateTracks(const std::vector<Detection> &detections, int frameWidth, int frameHeight)
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    // Get class names for filtering
    const auto& classNames = m_detector ? m_detector->classNames() : std::vector<std::string>();
    
    // Build list of tracked object detections with centroids
    struct DetectionWithCentroid {
        QPointF centroid;  // normalized
        int detectionIndex;
        QString label;
    };
    
    QVector<DetectionWithCentroid> trackedDetections;
    
    for (size_t i = 0; i < detections.size(); ++i) {
        const Detection& det = detections[i];
        
        // Track multiple classes: person, car, bicycle, dog, cat
        QString label = "unknown";
        if (det.classId >= 0 && det.classId < static_cast<int>(classNames.size())) {
            label = QString::fromStdString(classNames[det.classId]);
        }
        
        // Filter to track only these classes
        static const QStringList trackedClasses = {"person", "car", "bicycle", "dog", "cat"};
        if (!trackedClasses.contains(label)) {
            continue;
        }
        
        // Compute centroid in normalized coordinates
        double cx = (det.box.x + det.box.width / 2.0) / frameWidth;
        double cy = (det.box.y + det.box.height / 2.0) / frameHeight;
        
        DetectionWithCentroid dwc;
        dwc.centroid = QPointF(cx, cy);
        dwc.detectionIndex = static_cast<int>(i);
        dwc.label = label;
        trackedDetections.append(dwc);
    }
    
    // Track assignment: for each detection, find closest existing track
    QSet<int> updatedTrackIds;
    
    for (const auto& dwc : trackedDetections) {
        // Find closest track
        int bestTrackId = -1;
        double bestDistance = MAX_TRACK_DISTANCE;
        
        for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
            TrackState& track = it.value();
            
            // Compute Euclidean distance in normalized space
            double dx = track.centroid.x() - dwc.centroid.x();
            double dy = track.centroid.y() - dwc.centroid.y();
            double dist = std::sqrt(dx*dx + dy*dy);
            
            if (dist < bestDistance) {
                bestDistance = dist;
                bestTrackId = it.key();
            }
        }
        
        if (bestTrackId >= 0) {
            // Update existing track
            TrackState& track = m_tracks[bestTrackId];
            track.prevCentroid = track.centroid;
            track.centroid = dwc.centroid;
            track.lastSeenMs = currentTime;
            
            updatedTrackIds.insert(bestTrackId);
        } else {
            // Create new track
            TrackState newTrack;
            newTrack.id = m_nextTrackId++;
            newTrack.label = dwc.label;
            newTrack.centroid = dwc.centroid;
            newTrack.prevCentroid = dwc.centroid;  // Initially same as centroid
            newTrack.firstSeenMs = currentTime;
            newTrack.lastSeenMs = currentTime;
            newTrack.insideRoi = false;
            newTrack.loiterAlertSent = false;
            
            m_tracks[newTrack.id] = newTrack;
            updatedTrackIds.insert(newTrack.id);
        }
    }
    
    // Update ROI status and check loitering for all updated tracks
    for (int trackId : updatedTrackIds) {
        if (m_tracks.contains(trackId)) {
            updateRoiStatus(m_tracks[trackId], currentTime);
            checkLoitering(m_tracks[trackId], currentTime);
        }
    }
    
    // Check line crossing for all updated tracks
    if (m_hasTripwire) {
        for (int trackId : updatedTrackIds) {
            if (m_tracks.contains(trackId)) {
                checkLineCrossing(m_tracks[trackId], currentTime);
            }
        }
    }
    
    // Remove stale tracks
    cleanupStaleTracks(currentTime);
    
    // Log tracks for debugging
    if (!m_tracks.isEmpty()) {
        logTracks();
    }
}

void CaptureWorker::cleanupStaleTracks(qint64 currentTime)
{
    // Remove tracks not seen for TRACK_TIMEOUT_MS
    QList<int> toRemove;
    
    for (auto it = m_tracks.begin(); it != m_tracks.end(); ++it) {
        if (currentTime - it.value().lastSeenMs > TRACK_TIMEOUT_MS) {
            toRemove.append(it.key());
        }
    }
    
    for (int id : toRemove) {
        m_tracks.remove(id);
    }
}

void CaptureWorker::logTracks() const
{
    QStringList trackInfo;
    
    for (auto it = m_tracks.constBegin(); it != m_tracks.constEnd(); ++it) {
        const TrackState& track = it.value();
        qint64 ageMs = track.lastSeenMs - track.firstSeenMs;
        
        QString info = QString("Track %1: %2 @ (%3, %4) age=%5ms roi=%6")
            .arg(track.id)
            .arg(track.label)
            .arg(track.centroid.x(), 0, 'f', 3)
            .arg(track.centroid.y(), 0, 'f', 3)
            .arg(ageMs)
            .arg(track.insideRoi ? "yes" : "no");
        
        trackInfo.append(info);
    }
    
    if (!trackInfo.isEmpty()) {
        qDebug() << "[Tracking]" << trackInfo.join(" | ");
    }
}

double CaptureWorker::computeSideOfLine(const QPointF &point) const
{
    // Compute cross product: (p.x - x1)*(y2 - y1) - (p.y - y1)*(x2 - x1)
    // where line goes from (x1, y1) to (x2, y2)
    double x1 = m_tripwireStartNorm.x();
    double y1 = m_tripwireStartNorm.y();
    double x2 = m_tripwireEndNorm.x();
    double y2 = m_tripwireEndNorm.y();
    
    double side = (point.x() - x1) * (y2 - y1) - (point.y() - y1) * (x2 - x1);
    return side;
}

QString CaptureWorker::getCrossingDirection(double prevSide, double currSide) const
{
    // prevSide < 0 and currSide > 0 means crossing from negative side to positive side
    // prevSide > 0 and currSide < 0 means crossing from positive side to negative side
    
    if (prevSide < 0 && currSide > 0) {
        return "left to right";
    } else if (prevSide > 0 && currSide < 0) {
        return "right to left";
    }
    
    return "unknown";
}

void CaptureWorker::checkLineCrossing(TrackState &track, qint64 currentTime)
{
    // Only check if track has a valid previous centroid
    // (skip on first detection of track when prev == curr)
    if (track.centroid == track.prevCentroid) {
        return;
    }
    
    // Check debounce - don't spam alerts for same track
    if (currentTime - track.lastTripwireAlertMs < TRIPWIRE_ALERT_DEBOUNCE_MS) {
        return;
    }
    
    // Compute side of line for previous and current positions
    double prevSide = computeSideOfLine(track.prevCentroid);
    double currSide = computeSideOfLine(track.centroid);
    
    // Check if crossed (opposite signs and both above epsilon)
    if (std::abs(prevSide) > LINE_EPSILON && 
        std::abs(currSide) > LINE_EPSILON &&
        (prevSide * currSide < 0)) {
        
        // Line was crossed!
        QString direction = getCrossingDirection(prevSide, currSide);
        
        qDebug() << "[Line Crossing] Track" << track.id << "(" << track.label << ")"
                 << "crossed tripwire:" << direction
                 << "| prevSide:" << prevSide << "currSide:" << currSide;
        
        // Update debounce timestamp
        track.lastTripwireAlertMs = currentTime;
        
        // Emit signal with empty frame (we don't have it here, will get from captureFrame)
        emit trackCrossedTripwire(track.id, track.label, direction, cv::Mat());
    }
}

bool CaptureWorker::pointInRoi(const QPointF &point) const
{
    // If no ROI defined, return false
    if (!m_hasRoi || m_roiNorm.size() < 3) {
        return false;
    }
    
    // Ray-casting algorithm (point-in-polygon test)
    bool inside = false;
    int n = m_roiNorm.size();
    
    for (int i = 0, j = n - 1; i < n; j = i++) {
        double xi = m_roiNorm[i].x();
        double yi = m_roiNorm[i].y();
        double xj = m_roiNorm[j].x();
        double yj = m_roiNorm[j].y();
        
        if (((yi > point.y()) != (yj > point.y())) &&
            (point.x() < (xj - xi) * (point.y() - yi) / (yj - yi) + xi)) {
            inside = !inside;
        }
    }
    
    return inside;
}

void CaptureWorker::updateRoiStatus(TrackState &track, qint64 currentTime)
{
    // Check if track is currently inside ROI
    bool nowInside = pointInRoi(track.centroid);
    
    if (nowInside) {
        // Track is inside ROI
        if (!track.insideRoi) {
            // Track just entered ROI - mark entry time
            track.enteredRoiMs = currentTime;
            qDebug() << "[ROI Entry] Track" << track.id << "entered ROI at" << currentTime;
        }
        // Keep enteredRoiMs unchanged if already inside
    } else {
        // Track is outside ROI
        if (track.insideRoi) {
            // Track just exited ROI - reset loitering state
            qDebug() << "[ROI Exit] Track" << track.id << "exited ROI";
            track.enteredRoiMs = 0;
            track.loiterAlertSent = false;
        }
    }
    
    // Update current ROI status
    track.insideRoi = nowInside;
}

void CaptureWorker::checkLoitering(TrackState &track, qint64 currentTime)
{
    // Only check if track is inside ROI
    if (!track.insideRoi) {
        return;
    }
    
    // Only trigger once per visit
    if (track.loiterAlertSent) {
        return;
    }
    
    // Check if loitering threshold exceeded
    qint64 durationMs = currentTime - track.enteredRoiMs;
    
    if (durationMs >= LOITERING_THRESHOLD_MS) {
        qDebug() << "[Loitering] Track" << track.id << "(" << track.label << ")"
                 << "loitering detected - duration:" << durationMs << "ms";
        
        // Mark alert as sent
        track.loiterAlertSent = true;
        
        // Emit loitering signal
        emit loiteringDetected(track.id, track.label, durationMs, cv::Mat());
    }
}

// ============================================================================
// CameraStream Implementation
// ============================================================================

CameraStream::CameraStream(const QString &id, const QString &source, const QString &sourceType, const QString &name, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_source(source)
    , m_sourceType(sourceType)
    , m_running(false)
    , m_fps(0.0)
    , m_status("Stopped")
    , m_cameraName(name)
    , m_cameraIndex(-1)
    , m_isUrlSource(false)
    , m_motionEnabled(false)
    , m_motionSensitivity(50.0)
    , m_motionActive(false)
    , m_motionResetTimer(nullptr)
    , m_lastMotionAlertTime(0)
    , m_hasRoi(false)
    , m_roiAlertActive(false)
    , m_roiAlertResetTimer(nullptr)
    , m_lastRoiAlertTime(0)
    , m_hasTripwire(false)
    , m_tripwireAlertActive(false)
    , m_tripwireAlertResetTimer(nullptr)
    , m_lastTripwireAlertTime(0)
    , m_prevSide(0.0)
    , m_hasPrevSide(false)
    , m_detector(nullptr)
    , m_aiEnabled(false)
    , m_aiConfidenceThreshold(0.5)
    , m_workerThread(nullptr)
    , m_worker(nullptr)
    , m_autoSnapshotOnMotion(false)
    , m_autoSnapshotOnRoi(false)
    , m_autoSnapshotOnTripwire(false)
{
    // Determine source type and configure worker
    if (m_sourceType == "usb" || m_source.toInt() >= 0) {
        m_cameraIndex = m_source.toInt();
        m_isUrlSource = false;
    } else {
        m_sourceUrl = m_source;
        m_isUrlSource = true;
    }
    
    // Create worker thread
    m_workerThread = new QThread(this);
    m_worker = new CaptureWorker(m_cameraIndex);
    
    // Set source on worker
    if (m_isUrlSource) {
        m_worker->setSourceUrl(m_sourceUrl);
    }
    
    m_worker->moveToThread(m_workerThread);

    // Connect signals
    connect(m_worker, &CaptureWorker::frameCaptured, 
            this, &CameraStream::onFrameCaptured);
    connect(m_worker, &CaptureWorker::fpsUpdated, 
            this, &CameraStream::onFpsUpdated);
    connect(m_worker, &CaptureWorker::errorOccurred, 
            this, &CameraStream::onErrorOccurred);
    connect(m_worker, &CaptureWorker::motionDetected,
            this, &CameraStream::onMotionDetected);
    connect(m_worker, &CaptureWorker::roiMotionDetected,
            this, &CameraStream::onRoiMotionDetected);
    connect(m_worker, &CaptureWorker::tripwireCrossed,
            this, &CameraStream::onTripwireCrossed);
    connect(m_worker, &CaptureWorker::trackCrossedTripwire,
            this, &CameraStream::onTrackCrossedTripwire);
    connect(m_worker, &CaptureWorker::loiteringDetected,
            this, &CameraStream::onLoiteringDetected);
    connect(m_worker, &CaptureWorker::aiDetectionsReady,
            this, &CameraStream::onAIDetectionsReady);

    // Connect thread cleanup
    connect(m_workerThread, &QThread::finished, 
            m_worker, &QObject::deleteLater);

    // Create motion reset timer
    m_motionResetTimer = new QTimer(this);
    m_motionResetTimer->setSingleShot(true);
    m_motionResetTimer->setInterval(2000);
    connect(m_motionResetTimer, &QTimer::timeout,
            this, &CameraStream::resetMotionActive);
    
    // Create ROI alert reset timer
    m_roiAlertResetTimer = new QTimer(this);
    m_roiAlertResetTimer->setSingleShot(true);
    m_roiAlertResetTimer->setInterval(2000);
    connect(m_roiAlertResetTimer, &QTimer::timeout,
            this, &CameraStream::resetRoiAlertActive);
    
    // Create tripwire alert reset timer
    m_tripwireAlertResetTimer = new QTimer(this);
    m_tripwireAlertResetTimer->setSingleShot(true);
    m_tripwireAlertResetTimer->setInterval(2000);
    connect(m_tripwireAlertResetTimer, &QTimer::timeout,
            this, &CameraStream::resetTripwireAlertActive);

    // Start the thread
    m_workerThread->start();
}

CameraStream::~CameraStream()
{
    stop();
    
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
}

void CameraStream::start()
{
    if (m_running) {
        return;
    }

    m_running = true;
    m_status = "Starting...";
    emit runningChanged();
    emit statusChanged();

    // Invoke worker's start method on its thread
    QMetaObject::invokeMethod(m_worker, "start", Qt::QueuedConnection);
}

void CameraStream::stop()
{
    if (!m_running) {
        return;
    }

    m_running = false;
    m_status = "Stopped";
    m_fps = 0.0;
    emit runningChanged();
    emit statusChanged();
    emit fpsChanged();

    // Invoke worker's stop method on its thread
    QMetaObject::invokeMethod(m_worker, "stop", Qt::QueuedConnection);
}

void CameraStream::setAutoSnapshotOnMotion(bool enabled)
{
    if (m_autoSnapshotOnMotion != enabled) {
        m_autoSnapshotOnMotion = enabled;
        emit autoSnapshotOnMotionChanged();
        qDebug() << "Auto snapshot on motion:" << enabled << "for camera:" << m_cameraName;
    }
}

void CameraStream::setAutoSnapshotOnRoi(bool enabled)
{
    if (m_autoSnapshotOnRoi != enabled) {
        m_autoSnapshotOnRoi = enabled;
        emit autoSnapshotOnRoiChanged();
        qDebug() << "Auto snapshot on ROI:" << enabled << "for camera:" << m_cameraName;
    }
}

void CameraStream::setAutoSnapshotOnTripwire(bool enabled)
{
    if (m_autoSnapshotOnTripwire != enabled) {
        m_autoSnapshotOnTripwire = enabled;
        emit autoSnapshotOnTripwireChanged();
        qDebug() << "Auto snapshot on tripwire:" << enabled << "for camera:" << m_cameraName;
    }
}

void CameraStream::setSource(int cameraIndex)
{
    bool wasRunning = m_running;
    
    if (wasRunning) {
        stop();
    }

    m_cameraIndex = cameraIndex;
    m_isUrlSource = false;
    m_sourceUrl.clear();
    m_worker->setSource(cameraIndex);

    if (wasRunning) {
        start();
    }
}

void CameraStream::setSourceDevice(int deviceIndex)
{
    setSource(deviceIndex);
}

void CameraStream::setSourceUrl(const QString &url)
{
    bool wasRunning = m_running;
    
    if (wasRunning) {
        stop();
    }

    m_sourceUrl = url;
    m_isUrlSource = true;
    m_cameraIndex = -1;
    m_worker->setSourceUrl(url);

    if (wasRunning) {
        start();
    }
}

void CameraStream::onFrameCaptured(const QImage &frame)
{
    QMutexLocker locker(&m_frameMutex);
    m_currentFrame = frame;
    m_status = "Running";
    emit frameChanged();
    emit statusChanged();
}

void CameraStream::onFpsUpdated(double fps)
{
    m_fps = fps;
    emit fpsChanged();
}

void CameraStream::onErrorOccurred(const QString &error)
{
    qWarning() << "CameraStream error:" << error;
    m_status = QString("Error: %1").arg(error);
    emit statusChanged();
    
    // Auto-stop on error
    if (m_running) {
        stop();
    }
}

bool CameraStream::takeSnapshot()
{
    if (!m_running || m_currentFrame.isNull()) {
        qWarning() << "Cannot take snapshot: camera not running or no frame available";
        return false;
    }

    // Just emit the captured frame, don't save to disk
    emit snapshotCaptured(m_currentFrame);
    
    qDebug() << "Snapshot captured for camera:" << m_cameraName;
    return true;
}

bool CameraStream::saveSnapshot(const QString &targetDir)
{
    // This is the old method - keep for backward compatibility with motion detection
    // But we'll modify it to also emit the signal
    
    if (!m_running || m_currentFrame.isNull()) {
        qWarning() << "Cannot save snapshot: camera not running or no frame available";
        emit snapshotFailed("No frame available");
        return false;
    }

    QDir dir(targetDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString filename = QString("%1_%2.png").arg(m_cameraName).arg(timestamp);
    filename.replace(" ", "_");
    
    QString filePath = dir.filePath(filename);

    if (m_currentFrame.save(filePath, "PNG")) {
        qDebug() << "Snapshot saved:" << filePath;
        emit snapshotSaved(filePath);
        return true;
    } else {
        qWarning() << "Failed to save snapshot:" << filePath;
        emit snapshotFailed("Failed to write file");
        return false;
    }
}

void CameraStream::setMotionEnabled(bool enabled)
{
    if (m_motionEnabled == enabled) {
        return;
    }
    
    m_motionEnabled = enabled;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setMotionEnabled", Qt::QueuedConnection,
                              Q_ARG(bool, enabled));
    
    emit motionEnabledChanged();
    
    qDebug() << "Motion detection" << (enabled ? "enabled" : "disabled") << "for" << m_cameraName;
}

void CameraStream::setMotionSensitivity(double sensitivity)
{
    // Clamp to 0-100 range
    sensitivity = qBound(0.0, sensitivity, 100.0);
    
    if (qFuzzyCompare(m_motionSensitivity, sensitivity)) {
        return;
    }
    
    m_motionSensitivity = sensitivity;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setMotionSensitivity", Qt::QueuedConnection,
                              Q_ARG(double, sensitivity));
    
    emit motionSensitivityChanged();
}

void CameraStream::onMotionDetected(double score, const cv::Mat &frame)
{
    Q_UNUSED(frame);
    
    qDebug() << "Motion detected on" << m_cameraName << "- score:" << score;
    
    // Set motion active flag
    m_motionActive = true;
    emit motionActiveChanged();
    
    // Restart the reset timer
    m_motionResetTimer->start();
    
    // Emit motion detected signal for alert system
    emit motionDetected(score);
}

void CameraStream::resetMotionActive()
{
    if (m_motionActive) {
        m_motionActive = false;
        emit motionActiveChanged();
    }
}

void CameraStream::resetRoiAlertActive()
{
    if (m_roiAlertActive) {
        m_roiAlertActive = false;
        emit roiAlertActiveChanged();
    }
}

void CameraStream::resetTripwireAlertActive()
{
    if (m_tripwireAlertActive) {
        m_tripwireAlertActive = false;
        emit tripwireAlertActiveChanged();
    }
}

void CameraStream::onRoiMotionDetected(double score, const cv::Mat &frame)
{
    Q_UNUSED(frame);
    
    qDebug() << "ROI motion detected on" << m_cameraName << "- score:" << score;
    
    // Set ROI alert active flag
    m_roiAlertActive = true;
    emit roiAlertActiveChanged();
    
    // Restart the reset timer
    m_roiAlertResetTimer->start();
    
    // Emit ROI motion detected signal for alert system
    emit roiMotionDetected(score);
}

void CameraStream::onTripwireCrossed(int direction, const cv::Mat &frame)
{
    Q_UNUSED(frame);
    
    QString dirText = (direction > 0) ? "forward" : "backward";
    qDebug() << "Tripwire crossed on" << m_cameraName << "- direction:" << dirText;
    
    // Set tripwire alert active flag
    m_tripwireAlertActive = true;
    emit tripwireAlertActiveChanged();
    
    // Restart the reset timer
    m_tripwireAlertResetTimer->start();
    
    // Emit tripwire crossed signal for alert system
    emit tripwireCrossed(direction);
}

void CameraStream::onTrackCrossedTripwire(int trackId, const QString &label, const QString &direction, const cv::Mat &frame)
{
    Q_UNUSED(frame);
    
    qDebug() << "Track" << trackId << "(" << label << ") crossed tripwire on" 
             << m_cameraName << "- direction:" << direction;
    
    // Set tripwire alert active flag
    m_tripwireAlertActive = true;
    emit tripwireAlertActiveChanged();
    
    // Restart the reset timer
    m_tripwireAlertResetTimer->start();
    
    // Emit signal for alert system with full context
    emit trackCrossedTripwire(trackId, label, direction);
}

void CameraStream::onLoiteringDetected(int trackId, const QString &label, qint64 durationMs, const cv::Mat &frame)
{
    Q_UNUSED(frame);
    
    double durationSec = durationMs / 1000.0;
    qDebug() << "Track" << trackId << "(" << label << ") loitering detected on"
             << m_cameraName << "- duration:" << durationSec << "seconds";
    
    // Emit signal for alert system
    emit loiteringDetected(trackId, label, durationMs);
}

void CameraStream::setRoiPolygon(const QVector<QPointF> &normalizedPoints)
{
    m_roiNorm = normalizedPoints;
    m_hasRoi = !m_roiNorm.isEmpty();
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setRoiPolygon", Qt::QueuedConnection,
                              Q_ARG(QVector<QPointF>, normalizedPoints));
    
    qDebug() << "ROI set for" << m_cameraName << "with" << normalizedPoints.size() << "points";
}

void CameraStream::clearRoi()
{
    m_roiNorm.clear();
    m_hasRoi = false;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "clearRoi", Qt::QueuedConnection);
    
    qDebug() << "ROI cleared for" << m_cameraName;
}

void CameraStream::setTripwire(const QPointF &startNorm, const QPointF &endNorm)
{
    m_tripwireStartNorm = startNorm;
    m_tripwireEndNorm = endNorm;
    m_hasTripwire = true;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setTripwire", Qt::QueuedConnection,
                              Q_ARG(QPointF, startNorm), Q_ARG(QPointF, endNorm));
    
    qDebug() << "Tripwire set for" << m_cameraName;
}

void CameraStream::clearTripwire()
{
    m_tripwireStartNorm = QPointF();
    m_tripwireEndNorm = QPointF();
    m_hasTripwire = false;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "clearTripwire", Qt::QueuedConnection);
    
    qDebug() << "Tripwire cleared for" << m_cameraName;
}

void CameraStream::setAiEnabled(bool enabled)
{
    if (m_aiEnabled == enabled) {
        return;
    }
    
    m_aiEnabled = enabled;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setAiEnabled", Qt::QueuedConnection,
                              Q_ARG(bool, enabled));
    
    if (!enabled) {
        // Clear detections when AI is disabled
        {
            QMutexLocker locker(&m_detectionMutex);
            m_currentDetections.clear();
        }  // Unlock mutex BEFORE emitting signal
        emit detectionsChanged();  // Now safe to emit
    }
    
    emit aiEnabledChanged();
    qDebug() << "AI detection" << (enabled ? "enabled" : "disabled") << "for" << m_cameraName;
}

void CameraStream::setAiConfidenceThreshold(double threshold)
{
    threshold = qBound(0.0, threshold, 1.0);
    
    if (qFuzzyCompare(m_aiConfidenceThreshold, threshold)) {
        return;
    }
    
    m_aiConfidenceThreshold = threshold;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setAiConfidenceThreshold", Qt::QueuedConnection,
                              Q_ARG(double, threshold));
    
    emit aiConfidenceThresholdChanged();
}

void CameraStream::setObjectDetector(ObjectDetector *detector)
{
    m_detector = detector;
    
    // Update worker on its thread
    QMetaObject::invokeMethod(m_worker, "setObjectDetector", Qt::QueuedConnection,
                              Q_ARG(ObjectDetector*, detector));
    
    if (m_detector) {
        m_detector->setConfidenceThreshold(static_cast<float>(m_aiConfidenceThreshold));
    }
}

QVariantList CameraStream::detections() const
{
    QVariantList result;
    QMutexLocker locker(&m_detectionMutex);
    
    // Get current frame dimensions for normalization
    QMutexLocker frameLocker(&m_frameMutex);
    if (m_currentFrame.isNull()) {
        return result;
    }
    
    int frameWidth = m_currentFrame.width();
    int frameHeight = m_currentFrame.height();
    frameLocker.unlock();
    
    if (frameWidth > 0 && frameHeight > 0) {
        for (const auto &det : m_currentDetections) {
            const cv::Rect &box = det.box;
            
            QVariantMap detection;
            
            // Normalize coordinates to [0, 1] using double precision
            detection["x"] = static_cast<double>(box.x) / static_cast<double>(frameWidth);
            detection["y"] = static_cast<double>(box.y) / static_cast<double>(frameHeight);
            detection["width"] = static_cast<double>(box.width) / static_cast<double>(frameWidth);
            detection["height"] = static_cast<double>(box.height) / static_cast<double>(frameHeight);
            detection["classId"] = det.classId;
            detection["score"] = static_cast<double>(det.score);
            
            // Add class label with proper bounds checking
            QString label = "unknown";
            if (m_detector) {
                const auto& classNames = m_detector->classNames();
                if (det.classId >= 0 && det.classId < static_cast<int>(classNames.size())) {
                    label = QString::fromStdString(classNames[det.classId]);
                }
            }
            detection["label"] = label;
            
            result.append(detection);
        }
    }
    
    return result;
}

void CameraStream::onAIDetectionsReady(const std::vector<Detection> &detections)
{
    {
        QMutexLocker locker(&m_detectionMutex);
        m_currentDetections = detections;
    }
    emit detectionsChanged();
}