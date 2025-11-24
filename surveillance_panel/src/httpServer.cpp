#include "HttpServer.h"
#include "AlertLogModel.h"
#include "CameraManager.h"
#include "CameraStream.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QDebug>
#include <opencv2/opencv.hpp>

HttpServer::HttpServer(QObject *parent)
    : QObject(parent)
    , m_tcpServer(new QTcpServer(this))
    , m_alertLogModel(nullptr)
    , m_cameraManager(nullptr)
    , m_running(false)
    , m_port(0)
{
    connect(m_tcpServer, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
}

HttpServer::~HttpServer()
{
    stop();
}

void HttpServer::setAlertLogModel(AlertLogModel *model)
{
    m_alertLogModel = model;
}

void HttpServer::setCameraManager(CameraManager *manager)
{
    m_cameraManager = manager;
}

bool HttpServer::start(quint16 port)
{
    if (m_running) {
        qWarning() << "HTTP server already running on port" << m_port;
        return false;
    }

    if (!m_tcpServer->listen(QHostAddress::Any, port)) {
        qWarning() << "Failed to start HTTP server on port" << port << ":" 
                   << m_tcpServer->errorString();
        return false;
    }

    m_running = true;
    m_port = m_tcpServer->serverPort();
    
    qDebug() << "✓ HTTP server started on port" << m_port;
    emit serverStarted(m_port);
    
    return true;
}

void HttpServer::stop()
{
    if (!m_running) {
        return;
    }

    m_tcpServer->close();
    m_running = false;
    m_port = 0;
    
    qDebug() << "HTTP server stopped";
    emit serverStopped();
}

void HttpServer::onNewConnection()
{
    while (m_tcpServer->hasPendingConnections()) {
        QTcpSocket *socket = m_tcpServer->nextPendingConnection();
        
        connect(socket, &QTcpSocket::readyRead, this, &HttpServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &HttpServer::onDisconnected);
        
        m_pendingData[socket] = QByteArray();
    }
}

void HttpServer::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        return;
    }

    // Accumulate data
    m_pendingData[socket].append(socket->readAll());
    
    // Check if we have a complete HTTP request (ends with \r\n\r\n)
    if (!m_pendingData[socket].contains("\r\n\r\n")) {
        return;  // Wait for more data
    }

    QString method;
    QString path;
    QMap<QString, QString> headers;
    
    if (!parseHttpRequest(m_pendingData[socket], method, path, headers)) {
        sendError(socket, 400, "Bad Request");
        socket->disconnectFromHost();
        m_pendingData.remove(socket);
        return;
    }
    
    // Clear pending data for this socket
    m_pendingData.remove(socket);
    
    emit requestReceived(method, path);
    qDebug() << "HTTP" << method << path;
    
    handleRequest(socket, method, path, headers);
}

void HttpServer::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        m_pendingData.remove(socket);
        socket->deleteLater();
    }
}

bool HttpServer::parseHttpRequest(const QByteArray &data, QString &method, 
                                  QString &path, QMap<QString, QString> &headers)
{
    QString request = QString::fromUtf8(data);
    QStringList lines = request.split("\r\n");
    
    if (lines.isEmpty()) {
        return false;
    }
    
    // Parse request line: "GET /path HTTP/1.1"
    QStringList requestLine = lines[0].split(" ");
    if (requestLine.size() < 2) {
        return false;
    }
    
    method = requestLine[0];
    path = requestLine[1];
    
    // Parse headers
    for (int i = 1; i < lines.size(); ++i) {
        if (lines[i].isEmpty()) {
            break;
        }
        
        int colonPos = lines[i].indexOf(':');
        if (colonPos > 0) {
            QString key = lines[i].left(colonPos).trimmed();
            QString value = lines[i].mid(colonPos + 1).trimmed();
            headers[key] = value;
        }
    }
    
    return true;
}

void HttpServer::handleRequest(QTcpSocket *socket, const QString &method, 
                              const QString &path, const QMap<QString, QString> &headers)
{
    Q_UNUSED(headers);
    
    if (method != "GET") {
        sendError(socket, 405, "Method Not Allowed");
        socket->disconnectFromHost();
        return;
    }
    
    // Route handling
    if (path == "/ping") {
        handlePing(socket);
    }
    else if (path == "/alerts") {
        handleGetAlerts(socket);
    }
    else if (path.startsWith("/alerts/") && path.endsWith("/snapshot")) {
        // Extract alert ID: /alerts/<id>/snapshot
        QString alertId = path.mid(8, path.length() - 8 - 9);  // Remove "/alerts/" and "/snapshot"
        handleGetAlertSnapshot(socket, alertId);
    }
    else if (path == "/cameras") {
        handleGetCameras(socket);
    }
    else if (path.startsWith("/cameras/") && path.endsWith("/snapshot")) {
        // Extract camera ID: /cameras/<id>/snapshot
        QString cameraId = path.mid(9, path.length() - 9 - 9);  // Remove "/cameras/" and "/snapshot"
        handleGetCameraSnapshot(socket, cameraId);
    }
    else {
        sendNotFound(socket);
        socket->disconnectFromHost();
    }
}

void HttpServer::handlePing(QTcpSocket *socket)
{
    sendResponse(socket, 200, "OK", "text/plain", "ok");
    socket->disconnectFromHost();
}

void HttpServer::handleGetAlerts(QTcpSocket *socket)
{
    if (!m_alertLogModel) {
        sendError(socket, 503, "Alert service not available");
        socket->disconnectFromHost();
        return;
    }
    
    QJsonArray alertsArray;
    
    int count = m_alertLogModel->rowCount();
    
    // Return alerts in reverse order (newest first)
    for (int i = count - 1; i >= 0; --i) {
        QModelIndex index = m_alertLogModel->index(i, 0);
        
        QJsonObject alertObj;
        alertObj["id"] = m_alertLogModel->data(index, AlertLogModel::IdRole).toString();
        alertObj["timestamp"] = m_alertLogModel->data(index, AlertLogModel::TimestampRole)
                                    .toDateTime().toString("yyyy-MM-dd HH:mm:ss");
        alertObj["cameraName"] = m_alertLogModel->data(index, AlertLogModel::CameraNameRole).toString();
        alertObj["type"] = m_alertLogModel->data(index, AlertLogModel::TypeRole).toString();
        alertObj["message"] = m_alertLogModel->data(index, AlertLogModel::MessageRole).toString();
        alertObj["hasSnapshot"] = m_alertLogModel->data(index, AlertLogModel::HasImageRole).toBool();
        
        QString snapshotPath = m_alertLogModel->data(index, AlertLogModel::SnapshotPathRole).toString();
        if (!snapshotPath.isEmpty()) {
            alertObj["snapshotPath"] = snapshotPath;
        }
        
        alertsArray.append(alertObj);
    }
    
    QJsonDocument doc(alertsArray);
    sendJsonResponse(socket, 200, doc.toJson(QJsonDocument::Compact));
    socket->disconnectFromHost();
}

void HttpServer::handleGetAlertSnapshot(QTcpSocket *socket, const QString &alertId)
{
    if (!m_alertLogModel) {
        sendError(socket, 503, "Alert service not available");
        socket->disconnectFromHost();
        return;
    }
    
    // Find alert by ID
    int count = m_alertLogModel->rowCount();
    QImage snapshotImage;
    QString snapshotPath;
    bool found = false;
    
    for (int i = 0; i < count; ++i) {
        QModelIndex index = m_alertLogModel->index(i, 0);
        QString id = m_alertLogModel->data(index, AlertLogModel::IdRole).toString();
        
        if (id == alertId) {
            found = true;
            
            // Check if there's an in-memory image
            bool hasImage = m_alertLogModel->data(index, AlertLogModel::HasImageRole).toBool();
            if (hasImage) {
                // Get the image directly from the model
                // We need to access the internal Alert structure
                // For now, try to get from snapshot path first
                snapshotPath = m_alertLogModel->data(index, AlertLogModel::SnapshotPathRole).toString();
            } else {
                snapshotPath = m_alertLogModel->data(index, AlertLogModel::SnapshotPathRole).toString();
            }
            break;
        }
    }
    
    if (!found) {
        sendNotFound(socket, "Alert not found");
        socket->disconnectFromHost();
        return;
    }
    
    // Try to load from file path if available
    if (!snapshotPath.isEmpty() && QFile::exists(snapshotPath)) {
        QFile file(snapshotPath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray imageData = file.readAll();
            file.close();
            
            QString mimeType = "image/png";
            if (snapshotPath.endsWith(".jpg") || snapshotPath.endsWith(".jpeg")) {
                mimeType = "image/jpeg";
            }
            
            sendImageResponse(socket, imageData, mimeType);
            socket->disconnectFromHost();
            return;
        }
    }
    
    // If we have an in-memory image, we need a way to get it
    // For now, return 404 if file doesn't exist
    sendNotFound(socket, "Snapshot not available");
    socket->disconnectFromHost();
}

void HttpServer::handleGetCameras(QTcpSocket *socket)
{
    if (!m_cameraManager) {
        sendError(socket, 503, "Camera service not available");
        socket->disconnectFromHost();
        return;
    }
    
    QJsonArray camerasArray;
    
    // Check all 4 camera slots
    for (int i = 1; i <= 4; ++i) {
        if (m_cameraManager->cameraAvailable(i)) {
            QJsonObject camObj;
            
            // Generate camera ID (cam0, cam1, cam2, cam3)
            camObj["id"] = QString("cam%1").arg(i - 1);
            camObj["name"] = m_cameraManager->cameraName(i);
            camObj["type"] = m_cameraManager->cameraType(i);
            camObj["source"] = m_cameraManager->cameraSource(i);
            
            camerasArray.append(camObj);
        }
    }
    
    QJsonDocument doc(camerasArray);
    sendJsonResponse(socket, 200, doc.toJson(QJsonDocument::Compact));
    socket->disconnectFromHost();
}

void HttpServer::handleGetCameraSnapshot(QTcpSocket *socket, const QString &cameraId)
{
    if (!m_cameraManager) {
        sendError(socket, 503, "Camera service not available");
        socket->disconnectFromHost();
        return;
    }
    
    // Parse camera ID (cam0, cam1, cam2, cam3)
    if (!cameraId.startsWith("cam")) {
        sendNotFound(socket, "Invalid camera ID");
        socket->disconnectFromHost();
        return;
    }
    
    bool ok;
    int cameraIndex = cameraId.mid(3).toInt(&ok);
    if (!ok || cameraIndex < 0 || cameraIndex > 3) {
        sendNotFound(socket, "Invalid camera ID");
        socket->disconnectFromHost();
        return;
    }
    
    // Convert to 1-based index for CameraManager
    int managerIndex = cameraIndex + 1;
    
    if (!m_cameraManager->cameraAvailable(managerIndex)) {
        sendNotFound(socket, "Camera not available");
        socket->disconnectFromHost();
        return;
    }
    
    // Get camera stream
    CameraStream *stream = nullptr;
    switch(managerIndex) {
        case 1: stream = qobject_cast<CameraStream*>(m_cameraManager->camera1()); break;
        case 2: stream = qobject_cast<CameraStream*>(m_cameraManager->camera2()); break;
        case 3: stream = qobject_cast<CameraStream*>(m_cameraManager->camera3()); break;
        case 4: stream = qobject_cast<CameraStream*>(m_cameraManager->camera4()); break;
    }
    
    if (!stream) {
        sendNotFound(socket, "Camera stream not available");
        socket->disconnectFromHost();
        return;
    }
    
    // Get current frame
    QImage frame = stream->frame();
    
    if (frame.isNull()) {
        sendError(socket, 503, "No frame available");
        socket->disconnectFromHost();
        return;
    }
    
    // Convert QImage to JPEG
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    
    if (!frame.save(&buffer, "JPEG", 85)) {  // 85% quality
        sendError(socket, 500, "Failed to encode image");
        socket->disconnectFromHost();
        return;
    }
    
    sendImageResponse(socket, imageData, "image/jpeg");
    socket->disconnectFromHost();
}

void HttpServer::sendResponse(QTcpSocket *socket, int statusCode, const QString &statusText,
                              const QString &contentType, const QByteArray &body)
{
    QByteArray response;
    response.append(QString("HTTP/1.1 %1 %2\r\n").arg(statusCode).arg(statusText).toUtf8());
    response.append(QString("Content-Type: %1\r\n").arg(contentType).toUtf8());
    response.append(QString("Content-Length: %1\r\n").arg(body.size()).toUtf8());
    response.append("Connection: close\r\n");
    response.append("Access-Control-Allow-Origin: *\r\n");  // Enable CORS
    response.append("\r\n");
    response.append(body);
    
    socket->write(response);
    socket->flush();
}

void HttpServer::sendJsonResponse(QTcpSocket *socket, int statusCode, const QByteArray &json)
{
    QString statusText = (statusCode == 200) ? "OK" : "Error";
    sendResponse(socket, statusCode, statusText, "application/json", json);
}

void HttpServer::sendImageResponse(QTcpSocket *socket, const QByteArray &imageData, 
                                  const QString &mimeType)
{
    sendResponse(socket, 200, "OK", mimeType, imageData);
}

void HttpServer::sendNotFound(QTcpSocket *socket, const QString &message)
{
    QJsonObject errorObj;
    errorObj["error"] = message;
    
    QJsonDocument doc(errorObj);
    sendJsonResponse(socket, 404, doc.toJson(QJsonDocument::Compact));
}

void HttpServer::sendError(QTcpSocket *socket, int statusCode, const QString &message)
{
    QJsonObject errorObj;
    errorObj["error"] = message;
    errorObj["statusCode"] = statusCode;
    
    QJsonDocument doc(errorObj);
    
    QString statusText = message;
    sendResponse(socket, statusCode, statusText, "application/json", 
                doc.toJson(QJsonDocument::Compact));
}