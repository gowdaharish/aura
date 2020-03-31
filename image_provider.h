#pragma once

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QUrl>
#include <QQuickImageProvider>

using namespace cv;

class ImageWorkerThread;

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT

    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(double beta READ beta WRITE setBeta NOTIFY betaChanged)
    Q_PROPERTY(double gamma READ gamma WRITE setGamma NOTIFY gammaChanged)

public:
    ImageProvider(QObject* parent = nullptr);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    QImage image() const {return _image;}
    void setImage(const QImage& image);
    double alpha() const {return _alpha;}
    void setAlpha(const double alpha);
    double beta() const {return _beta;}
    void setBeta(const double beta);
    double gamma() const {return _gamma;}
    void setGamma(const double gamma);

public slots:
    void initiate();
    void terminate();
    void handleResults(const QImage& image);

signals:
    void imageChanged(const QImage& image);
    void alphaChanged(const int alpha);
    void betaChanged(const double beta);
    void gammaChanged(const double gamma);

private:
    void createWorkerConnections();

    Mat _imgOriginal, _imgCorrected, _imgGammaCorrected;
    ImageWorkerThread* _workerThread;
    QImage _image;
    QPixmap _pixmap;
    QImage::Format _format = QImage::Format_RGB888;
    QString _path;
    double _alpha = 1;
    double _beta = 1;
    double _gamma = 1;
};
