#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include <QObject>
#include <QPixmap>
#include <QThread>

using namespace cv;

class ImageProvider;

class ImageWorkerThread : public QObject
{
    Q_OBJECT

public:
    ImageWorkerThread(QObject* parent = nullptr);

public slots:
    void setImage(const QImage& path);
    void setOriginalImage(const QImage& image);
    void setAlpha(const double alpha);
    void setBeta(const double beta);
    void setGamma(const double gamma);
    void stop();

signals:
    void resultReady(const QImage& image);

private:
    void basicLinearTransform(const Mat& img, const double alpha_, const int beta);
    void gammaCorrection(const Mat& img, const double gamma);
    void ABTransform(const double alpha, const double beta);

    Mat _matCorrected;
    QImage _image, _originalImage;
    QPixmap _pixmap;
    bool _stop = false;
    QString _path;
    int _alpha = 1;
    double _beta = 1;
    double _gamma = 1;
};
