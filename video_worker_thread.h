#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include <QObject>
#include <QPixmap>
#include <QThread>

using namespace cv;

class VideoProvider;

class VideoWorkerThread : public QThread
{
    Q_OBJECT

public:
    VideoWorkerThread(QObject* parent = nullptr);
    bool running() const {return  !_stop;}

public slots:
    void setFormat(const QImage::Format format);
    void setFps(const int fps);
    void setResolution(const QString& resolution);
    void setBrightness(const int brightness);
    void setContrast(const int contrast);
    void setSaturation(const int saturation);
    void setExposure(const int exposure);
    void stop();

signals:
    void resultReady(const QImage& image);

protected:
    void run() override;

private:
    QPixmap pixmap;
    bool _stop = false;
    QImage::Format _format = QImage::Format_RGB888;
    int _fps = 30;
    double _resolutionWidth = 640;
    double _resolutionHeight = 480;
    int _brightness = 1;
    int _contrast = 1;
    int _saturation = 1;
    int _exposure = 1;
    int _selectedTab = 0;
};
