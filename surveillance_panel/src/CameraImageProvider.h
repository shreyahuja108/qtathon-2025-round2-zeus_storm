#ifndef CAMERAIMAGEPROVIDER_H
#define CAMERAIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include "CameraStream.h"

/**
 * @brief Image provider for displaying camera frames in QML
 */
class CameraImageProvider : public QQuickImageProvider
{
public:
    CameraImageProvider();
    
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    
    void setCameraStream(CameraStream *stream);

private:
    CameraStream *m_cameraStream;
};

#endif // CAMERAIMAGEPROVIDER_H