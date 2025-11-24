#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCoreApplication>
#include <QDir>
#include <iostream>
#include "CameraStream.h"
#include "CameraImageProvider.h"
#include "CameraManager.h"
#include "AlertLogModel.h"
#include "HttpServer.h"

int main(int argc, char *argv[])
{
    // Create the Qt application
    QGuiApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("SurveillancePanel");
    app.setOrganizationName("QTHackathon");
    app.setApplicationVersion("1.0.0");

    // Create QML engine
    QQmlApplicationEngine engine;

    // Create camera manager (loads config and creates camera streams)
    CameraManager cameraManager;
    
    // Create alert log model
    AlertLogModel alertLog;
    
    // Set up default directories
    QDir appDir(QCoreApplication::applicationDirPath());
    
    // Snapshots directory
    appDir.mkpath("snapshots");
    QString snapshotsDir = appDir.filePath("snapshots");
    
    // Logs directory
    appDir.mkpath("logs");
    QString logsDir = appDir.filePath("logs");

    // Create and register image providers for each camera
    for (int i = 1; i <= 4; ++i) {
        if (cameraManager.cameraAvailable(i)) {
            CameraImageProvider *provider = new CameraImageProvider();
            
            // Get the camera stream for this index
            CameraStream *stream = nullptr;
            switch(i) {
                case 1: stream = qobject_cast<CameraStream*>(cameraManager.camera1()); break;
                case 2: stream = qobject_cast<CameraStream*>(cameraManager.camera2()); break;
                case 3: stream = qobject_cast<CameraStream*>(cameraManager.camera3()); break;
                case 4: stream = qobject_cast<CameraStream*>(cameraManager.camera4()); break;
            }
            
            if (stream) {
                provider->setCameraStream(stream);
                engine.addImageProvider(QString("camera%1").arg(i), provider);
                
                // Connect frameChanged to trigger image provider updates
                QObject::connect(stream, &CameraStream::frameChanged, [&engine, i]() {
                    QString propName = QString("frameCounter%1").arg(i);
                    engine.rootContext()->setContextProperty(propName, 
                        engine.rootContext()->contextProperty(propName).toInt() + 1);
                });
                
                // Connect snapshot captured signal to alert log (for manual snapshots)
                QObject::connect(stream, &CameraStream::snapshotCaptured, 
                                 &alertLog, [&alertLog, stream](const QImage &image) {
                    alertLog.addSnapshotAlert(stream->cameraName(), image);
                });
                
                // Connect motion detection to alert log
                QObject::connect(stream, &CameraStream::motionDetected,
                                 &alertLog, [&alertLog, stream](double score) {
                    // Always create motion alert
                    QString message = QString("Motion detected (score: %1)").arg(
                        QString::number(score, 'f', 1));
                    alertLog.addMotionAlert(stream->cameraName(), message, "");
                    
                    // If auto-snapshot is enabled, also create a snapshot alert
                    if (stream->autoSnapshotOnMotion() && !stream->frame().isNull()) {
                        alertLog.addSnapshotAlert(stream->cameraName(), stream->frame());
                    }
                });
                
                // Connect ROI motion detection to alert log
                QObject::connect(stream, &CameraStream::roiMotionDetected,
                                 &alertLog, [&alertLog, stream](double score) {
                    // Always create ROI motion alert
                    QString message = QString("Motion in ROI (score: %1)").arg(
                        QString::number(score, 'f', 1));
                    alertLog.addRoiMotionAlert(stream->cameraName(), message, "");
                    
                    // If auto-snapshot is enabled, also create a snapshot alert
                    if (stream->autoSnapshotOnRoi() && !stream->frame().isNull()) {
                        alertLog.addSnapshotAlert(stream->cameraName(), stream->frame());
                    }
                });
                
                // Connect tripwire crossing to alert log
                QObject::connect(stream, &CameraStream::tripwireCrossed,
                                 &alertLog, [&alertLog, stream](int direction) {
                    // Always create tripwire alert
                    QString dirText = (direction > 0) ? "forward" : "backward";
                    QString message = QString("Tripwire crossed (%1)").arg(dirText);
                    alertLog.addTripwireAlert(stream->cameraName(), message, "", direction);
                    
                    // If auto-snapshot is enabled, also create a snapshot alert
                    if (stream->autoSnapshotOnTripwire() && !stream->frame().isNull()) {
                        alertLog.addSnapshotAlert(stream->cameraName(), stream->frame());
                    }
                });
                
                // Connect track-based tripwire crossing to alert log
                QObject::connect(stream, &CameraStream::trackCrossedTripwire,
                                 &alertLog, [&alertLog, stream](int trackId, const QString &label, const QString &direction) {
                    // Create tripwire alert with track and direction info
                    QString message = QString("Track %1 (%2) crossed tripwire (%3)")
                        .arg(trackId)
                        .arg(label)
                        .arg(direction);
                    alertLog.addTripwireAlert(stream->cameraName(), message, "", 
                                             direction == "left to right" ? 1 : -1);
                    
                    // If auto-snapshot is enabled, also create a snapshot alert
                    if (stream->autoSnapshotOnTripwire() && !stream->frame().isNull()) {
                        alertLog.addSnapshotAlert(stream->cameraName(), stream->frame());
                    }
                });
                
                // Connect loitering detection to alert log
                QObject::connect(stream, &CameraStream::loiteringDetected,
                                 &alertLog, [&alertLog, stream](int trackId, const QString &label, qint64 durationMs) {
                    double durationSec = durationMs / 1000.0;
                    QString message = QString("Track %1 (%2) loitering: stayed in ROI for %3 seconds")
                        .arg(trackId)
                        .arg(label)
                        .arg(durationSec, 0, 'f', 1);
                    alertLog.addLoiteringAlert(stream->cameraName(), message, "");
                    
                    // Auto-snapshot if ROI snapshot enabled
                    if (stream->autoSnapshotOnRoi() && !stream->frame().isNull()) {
                        alertLog.addSnapshotAlert(stream->cameraName(), stream->frame());
                    }
                });
                
                // Initialize frame counter
                engine.rootContext()->setContextProperty(QString("frameCounter%1").arg(i), 0);
            }
        }
    }

    // Expose camera manager to QML
    engine.rootContext()->setContextProperty("cameraManager", &cameraManager);
    
    // Expose alert log to QML
    engine.rootContext()->setContextProperty("alertLog", &alertLog);
    
    // Expose directory paths to QML
    engine.rootContext()->setContextProperty("snapshotsDir", snapshotsDir);
    engine.rootContext()->setContextProperty("logsDir", logsDir);

    // ============================================================================
    // HTTP SERVER SETUP
    // ============================================================================
    
    // Create HTTP server
    HttpServer *httpServer = new HttpServer(&app);
    
    // Set data providers
    httpServer->setAlertLogModel(&alertLog);
    httpServer->setCameraManager(&cameraManager);
    
    // Start server on port 8080
    if (httpServer->start(8080)) {
        std::cout << "✓ HTTP API available at:" << std::endl;
        std::cout << "  http://localhost:8080/ping" << std::endl;
        std::cout << "  http://localhost:8080/alerts" << std::endl;
        std::cout << "  http://localhost:8080/cameras" << std::endl;
        std::cout << "  http://localhost:8080/cameras/cam0/snapshot" << std::endl;
        std::cout << "  http://localhost:8080/alerts/<id>/snapshot" << std::endl;
    } else {
        std::cerr << "✗ Failed to start HTTP server" << std::endl;
    }
    
    // Expose HTTP server to QML (optional, for status display)
    engine.rootContext()->setContextProperty("httpServer", httpServer);
    
    // ============================================================================

    // Load the main QML file using the QML module URI path
    const QUrl url(QStringLiteral("qrc:/SurveillancePanel/qml/Main.qml"));
    
    // Connect to handle QML object creation
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                std::cerr << "Error: Failed to load QML file: " 
                          << url.toString().toStdString() << std::endl;
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection
    );

    // Load the QML file
    engine.load(url);

    // Check if engine loaded successfully
    if (engine.rootObjects().isEmpty()) {
        std::cerr << "Error: No root objects found in QML" << std::endl;
        return -1;
    }

    // Start the event loop
    return app.exec();
}