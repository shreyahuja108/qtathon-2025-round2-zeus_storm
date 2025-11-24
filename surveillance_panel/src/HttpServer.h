#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QMap>
#include <functional>

class AlertLogModel;
class CameraManager;

/**
 * @brief Lightweight HTTP server for exposing alerts and camera snapshots via REST API
 */
class HttpServer : public QObject
{
    Q_OBJECT

public:
    explicit HttpServer(QObject *parent = nullptr);
    ~HttpServer();

    // Set data providers
    void setAlertLogModel(AlertLogModel *model);
    void setCameraManager(CameraManager *manager);

    // Start/stop server
    bool start(quint16 port = 8080);
    void stop();
    
    bool isRunning() const { return m_running; }
    quint16 port() const { return m_port; }

signals:
    void serverStarted(quint16 port);
    void serverStopped();
    void requestReceived(const QString &method, const QString &path);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    // HTTP request handling
    void handleRequest(QTcpSocket *socket, const QString &method, 
                      const QString &path, const QMap<QString, QString> &headers);
    
    // Route handlers
    void handlePing(QTcpSocket *socket);
    void handleGetAlerts(QTcpSocket *socket);
    void handleGetAlertSnapshot(QTcpSocket *socket, const QString &alertId);
    void handleGetCameras(QTcpSocket *socket);
    void handleGetCameraSnapshot(QTcpSocket *socket, const QString &cameraId);
    
    // HTTP response helpers
    void sendResponse(QTcpSocket *socket, int statusCode, const QString &statusText,
                     const QString &contentType, const QByteArray &body);
    void sendJsonResponse(QTcpSocket *socket, int statusCode, const QByteArray &json);
    void sendImageResponse(QTcpSocket *socket, const QByteArray &imageData, 
                          const QString &mimeType);
    void sendNotFound(QTcpSocket *socket, const QString &message = "Not Found");
    void sendError(QTcpSocket *socket, int statusCode, const QString &message);
    
    // Request parsing
    bool parseHttpRequest(const QByteArray &data, QString &method, 
                         QString &path, QMap<QString, QString> &headers);

    QTcpServer *m_tcpServer;
    AlertLogModel *m_alertLogModel;
    CameraManager *m_cameraManager;
    
    bool m_running;
    quint16 m_port;
    
    // Track pending data for each socket
    QMap<QTcpSocket*, QByteArray> m_pendingData;
};

#endif // HTTPSERVER_H