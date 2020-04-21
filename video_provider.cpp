#include "video_provider.h"
#include "video_worker_thread.h"

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QCoreApplication>
#include <QCameraInfo>

VideoProvider::VideoProvider(QObject* parent) : QObject{parent}, QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

VideoProvider::~VideoProvider()
{
    if (!_workerThread)
        return;

    _workerThread->quit();
    _workerThread->wait();
}

void VideoProvider::initiate()
{
    if (_running)
        return;

    _workerThread = new VideoWorkerThread{this};
    connect(_workerThread, &VideoWorkerThread::resultReady, this, &VideoProvider::handleResults);
    connect(_workerThread, &VideoWorkerThread::finished, _workerThread, &QObject::deleteLater);
    connect(this, &VideoProvider::stopWorker, _workerThread, &VideoWorkerThread::stop);
    connect(this, &VideoProvider::formatChanged, _workerThread, &VideoWorkerThread::setFormat);
    connect(this, &VideoProvider::fpsChanged, _workerThread, &VideoWorkerThread::setFps);
    connect(this, &VideoProvider::resolutionChanged, _workerThread, &VideoWorkerThread::setResolution);
    connect(this, &VideoProvider::brightnessChanged, _workerThread, &VideoWorkerThread::setBrightness);
    connect(this, &VideoProvider::contrastChanged, _workerThread, &VideoWorkerThread::setContrast);
    connect(this, &VideoProvider::saturationChanged, _workerThread, &VideoWorkerThread::setSaturation);
    connect(this, &VideoProvider::exposureChanged, _workerThread, &VideoWorkerThread::setExposure);
    _workerThread->start();
}

void VideoProvider::terminate()
{
    if (!_running)
        return;

    setRunning(false);
    emit stopWorker();
}

// helps us to break the image capture loop
void VideoProvider::setRunning(const bool running)
{
    if (running == _running)
        return;

    _running = running;
    emit runningChanged(_running);
}

// an override function to send the image pixmaps
QPixmap VideoProvider::requestPixmap(const QString& /*id*/, QSize* /*size*/, const QSize& /*requestedSize*/)
{
    return pixmap.fromImage(frameImage);
}

void VideoProvider::handleResults(const QImage& image)
{
    frameImage = image;

    // only do this when the worker is not active
    if (!_running && _workerThread->running())
        setRunning(true);
}

QString VideoProvider::getCameraName() const
{
    const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    if (!cameras.isEmpty())
        return cameras.first().description();

    return QString{"Default"};
}

void VideoProvider::setFormat(const QImage::Format format)
{
    if (format == _format)
        return;

    _format = format;
    emit formatChanged(_format);
}

void VideoProvider::setFps(const int fps)
{
    if (fps == _fps)
        return;

    _fps = fps;
    emit fpsChanged(_fps);
}

void VideoProvider::setResolution(const QString& resolution)
{
    if (resolution == _resolution)
        return;

    _resolution = resolution;
    emit resolutionChanged(_resolution);
}

void VideoProvider::setBrightness(const int brightness)
{
    if (brightness == _brightness)
        return;

    _brightness = brightness;
    emit brightnessChanged(_brightness);
}

void VideoProvider::setContrast(const int contrast)
{
    if (contrast == _contrast)
        return;

    _contrast = contrast;
    emit contrastChanged(_contrast);
}

void VideoProvider::setSaturation(const int saturation)
{
    if (saturation == _saturation)
        return;

    _saturation = saturation;
    emit saturationChanged(_saturation);
}

void VideoProvider::setExposure(const int exposure)
{
    if (exposure == _exposure)
        return;

    _exposure = exposure;
    emit saturationChanged(_exposure);
}
