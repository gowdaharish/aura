#include "video_worker_thread.h"

#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QCameraInfo>

VideoWorkerThread::VideoWorkerThread(QObject* parent) : QThread{parent}
{}

VideoWorkerThread::~VideoWorkerThread()
{
}

void VideoWorkerThread::run()
{
    qDebug() << "iniitiating capture" << endl;

    Mat frame;

    //intialize video capture
    VideoCapture cap;

    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API

    // open selected camera using selected API
    cap.open(deviceID + apiID);

    // check if we succeeded
    if (!cap.isOpened()) {
        qCritical() << "ERROR! Unable to open camera\n";
        return;
    }

    qDebug() << "start grabbing" << endl;

    while (!_stop)
    {
        // update fps if it has changed from outside
        cap.set(CAP_PROP_FPS, _fps);

        // update resolution if it has changed from outside
        cap.set(CAP_PROP_FRAME_WIDTH, _resolutionWidth);
        cap.set(CAP_PROP_FRAME_HEIGHT, _resolutionHeight);

        // update saturation
        cap.set(CAP_PROP_SATURATION, _saturation);

        // update exposure
        cap.set(CAP_PROP_EXPOSURE, _exposure);

        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);

        // check if we succeeded
        if (frame.empty()) {
            qCritical() << "ERROR! blank frame grabbed\n";
            continue;
        }

        // update brightness
        frame.convertTo(frame, -1, _brightness, 1);

        // update contrast
        frame.convertTo(frame, -1, 1, _contrast);

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage image(frame.data, frame.cols, frame.rows, _format);
        emit resultReady(image);
    }

    qDebug() << "finished" << endl;
}

void VideoWorkerThread::setFormat(const QImage::Format format)
{
    if (format == _format)
        return;

    _format = format;
}

void VideoWorkerThread::setFps(const int fps)
{
    if (fps == _fps)
        return;

    _fps = fps;
}

void VideoWorkerThread::setResolution(const QString& resolution)
{
    const auto res = resolution.split(":");
    _resolutionWidth = res[0].toDouble();
    _resolutionHeight = res[1].toDouble();
}

void VideoWorkerThread::setBrightness(const int brightness)
{
    if (brightness == _brightness)
        return;

    _brightness = brightness;
}

void VideoWorkerThread::setContrast(const int contrast)
{
    if (contrast == _contrast)
        return;

    _contrast = contrast;
}

void VideoWorkerThread::setSaturation(const int saturation)
{
    if (saturation == _saturation)
        return;

    _saturation = saturation;
}

void VideoWorkerThread::setExposure(const int exposure)
{
    if (exposure == _exposure)
        return;

    _exposure = exposure;
}

void VideoWorkerThread::stop()
{
    qDebug() << "stop grabbing" << endl;
    _stop = true;
}
