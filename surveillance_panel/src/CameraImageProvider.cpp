#include "CameraImageProvider.h"

CameraImageProvider::CameraImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_cameraStream(nullptr)
{
}

QImage CameraImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(requestedSize);

    if (!m_cameraStream) {
        QImage placeholder(320, 240, QImage::Format_RGB888);
        placeholder.fill(Qt::black);
        if (size) {
            *size = placeholder.size();
        }
        return placeholder;
    }

    QImage frame = m_cameraStream->frame();
    
    if (frame.isNull()) {
        QImage placeholder(320, 240, QImage::Format_RGB888);
        placeholder.fill(Qt::darkGray);
        if (size) {
            *size = placeholder.size();
        }
        return placeholder;
    }

    if (size) {
        *size = frame.size();
    }

    return frame;
}

void CameraImageProvider::setCameraStream(CameraStream *stream)
{
    m_cameraStream = stream;
}