#include "image_worker_thread.h"
#include "image_provider.h"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QCameraInfo>

using namespace cv;

#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#error Some of QImage's formats are endian-dependant. This file assumes little endian. See comment at top of header.
#endif

namespace {

QImage::Format IMAGE_FORMAT = QImage::Format_RGB888;

QImage matToImage(const Mat& mat)
{
    cvtColor(mat, mat, COLOR_BGR2RGB);
    QImage image(mat.data, mat.cols, mat.rows, IMAGE_FORMAT);
    return image;
}

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true )
{
    switch ( inImage.format() )
    {
    // 8-bit, 4 channel
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB32:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        cv::Mat  matNoAlpha;

        cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
    }

        // 8-bit, 3 channel
    case QImage::Format_RGB888:
    {
        if ( !inCloneImageData )
        {
            qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        QImage   swapped = inImage.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                        CV_8UC3,
                        const_cast<uchar*>(swapped.bits()),
                        static_cast<size_t>(swapped.bytesPerLine())
                        ).clone();
    }

        // 8-bit, 1 channel
    case QImage::Format_Indexed8:
    {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC1,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
    }

    default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}
}

ImageWorkerThread::ImageWorkerThread(QObject * /*parent*/)
{
}

void ImageWorkerThread::run()
{
    while (true)
    {}
}

void ImageWorkerThread::setOriginalImage(const QImage& image)
{
    if (_originalImage == image)
        return;

    _originalImage = image;
}

void ImageWorkerThread::setImage(const QImage& image)
{
    if (image == _image)
        return;

    _image = image;

    _mat = QImageToCvMat(_image);
    _matCorrected = Mat(_mat.rows, _mat.cols, _mat.type());
    emit resultReady(_image);
}

void ImageWorkerThread::ABTransform(const double alpha, const double beta)
{
    Mat mat = QImageToCvMat(_originalImage);
    mat.convertTo(_matCorrected, -1, alpha, beta);
    setImage(matToImage(_matCorrected));
}

void ImageWorkerThread::setAlpha(const double alpha)
{
    if (_originalImage.isNull())
        return;

    if (alpha == _alpha)
        return;

    _alpha = alpha;
    const auto beta = 1;

    ABTransform(_alpha, beta);
}

void ImageWorkerThread::setBeta(const double beta)
{
    if (_originalImage.isNull())
        return;

    if (beta == _beta)
        return;

    _beta = beta;
    const auto alpha = 1;

    ABTransform(alpha, beta);
}

void ImageWorkerThread::setGamma(const double gamma)
{
    if (_originalImage.isNull())
        return;

    if (gamma == _gamma)
        return;

    _gamma = gamma;
    Mat mat = QImageToCvMat(_originalImage);
    gammaCorrection(mat, _gamma);
    setImage(matToImage(_matCorrected));
}

void ImageWorkerThread::gammaCorrection(const Mat& img, const double gamma)
{
    Q_ASSERT(gamma >= 0);
    Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);

    _matCorrected = img.clone();
    LUT(img, lookUpTable, _matCorrected);
}
