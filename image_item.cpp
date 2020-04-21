#include "image_item.h"
#include "image_worker_thread.h"

#include <QFileDialog>
#include <QQuickImageProvider>

ImageItem::ImageItem(QQuickPaintedItem* parent) : QQuickPaintedItem{parent}, _workerThread{this}
{
    connect(&_workerThread, &ImageWorkerThread::resultReady, this, &ImageItem::handleResults);
    connect(this, &ImageItem::stopWorker, &_workerThread, &ImageWorkerThread::stop);
    connect(this, &ImageItem::imageChanged, &_workerThread, &ImageWorkerThread::setImage);
    connect(this, &ImageItem::imageChanged, &_workerThread, &ImageWorkerThread::setOriginalImage);
    connect(this, &ImageItem::alphaChanged, &_workerThread, &ImageWorkerThread::setAlpha);
    connect(this, &ImageItem::betaChanged, &_workerThread, &ImageWorkerThread::setBeta);
    connect(this, &ImageItem::gammaChanged, &_workerThread, &ImageWorkerThread::setGamma);
}

void ImageItem::readImage()
{
    _fileName = QFileDialog::getOpenFileName({}, tr("Open Image"), "/Users/hariahgangaraji/Desktop", tr("Image Files (*.png *.jpg *.bmp)"));

    if (_fileName.isEmpty())
    {
        setImage(QImage{});
        qDebug() << "failed to load the image" << endl;
    }

    setImage(QImage{_fileName});
}

QString ImageItem::fileNmae() const
{
    if (_fileName.isEmpty())
        return QString{"None selected"};

    return _fileName;
}

void ImageItem::handleResults(const QImage& image)
{
    _currentImage = image;
    update();
}

void ImageItem::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    QImage scaled = _currentImage.scaledToHeight(bounding_rect.height());
    QPointF center = bounding_rect.center() - scaled.rect().center();

    if(center.x() < 0)
        center.setX(0);
    if(center.y() < 0)
        center.setY(0);
    painter->drawImage(center, scaled);
}

void ImageItem::setImage(const QImage& image)
{
    if (image == _currentImage)
        return;

    _currentImage = image;
    emit imageChanged(_currentImage);
    update();
}

QImage ImageItem::image() const
{
    return _currentImage;
}

void ImageItem::setProcessingEnabled(const bool processingEnabled)
{
    if (processingEnabled == _processingEnabled)
        return;

    _processingEnabled = processingEnabled;
    emit processingEnabledChanged(_processingEnabled);
}

void ImageItem::setAlpha(const double alpha)
{
    if (alpha == _alpha)
        return;

    _alpha = alpha;
    emit alphaChanged(_alpha);
}

void ImageItem::setBeta(const double beta)
{
    if (beta == _beta)
        return;

    _beta = beta;
    emit betaChanged(_beta);
}

void ImageItem::setGamma(const double gamma)
{
    if (gamma == _gamma)
        return;

    _gamma = gamma;
    emit gammaChanged(_gamma);
}

void ImageItem::clearImage()
{
    setImage(QImage{});
    emit imageCleared();
}

void ImageItem::terminate()
{
    emit stopWorker();
}
