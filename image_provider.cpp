#include "image_provider.h"
#include "image_worker_thread.h"

#include <QDebug>
#include <QUrl>
#include <QDir>

ImageProvider::ImageProvider(QObject* parent) : QObject{parent}, QQuickImageProvider(QQuickImageProvider::Pixmap)
{
//    createWorkerConnections();
}

void ImageProvider::createWorkerConnections()
{
    _workerThread = new ImageWorkerThread;
    connect(_workerThread, &ImageWorkerThread::resultReady, this, &ImageProvider::handleResults);
    connect(_workerThread, &ImageWorkerThread::finished, _workerThread, &QObject::deleteLater);
    connect(this, &ImageProvider::imageChanged, _workerThread, &ImageWorkerThread::setImage);
    connect(this, &ImageProvider::alphaChanged, _workerThread, &ImageWorkerThread::setAlpha);
    connect(this, &ImageProvider::betaChanged, _workerThread, &ImageWorkerThread::setBeta);
    connect(this, &ImageProvider::gammaChanged, _workerThread, &ImageWorkerThread::setGamma);
}

void ImageProvider::initiate()
{
    createWorkerConnections();
    _workerThread->start();
}

void ImageProvider::handleResults(const QImage& image)
{
    _image = image;
}

// an override function to send the image pixmaps
QPixmap ImageProvider::requestPixmap(const QString& /*id*/, QSize* /*size*/, const QSize& /*requestedSize*/)
{
    return _pixmap.fromImage(_image);
}

void ImageProvider::terminate()
{
    qDebug() << "stoppping image provider object" << endl;
}

void ImageProvider::setImage(const QImage& image)
{
    if (image == _image)
        return;

    _image = image;
    emit imageChanged(_image);
}

void ImageProvider::setAlpha(const double alpha)
{
    if (alpha == _alpha)
        return;

    _alpha = alpha;
    emit alphaChanged(_alpha);
}

void ImageProvider::setBeta(const double beta)
{
    if (beta == _beta)
        return;

    _beta = beta;
    emit betaChanged(_beta);
}

void ImageProvider::setGamma(const double gamma)
{
    if (gamma == _gamma)
        return;

    _gamma = gamma;
    emit gammaChanged(_gamma);
}
