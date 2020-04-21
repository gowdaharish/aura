#pragma once

#include "image_worker_thread.h"

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

//class ImageWorkerThread;

class ImageItem : public QQuickPaintedItem
{
Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(double alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(double beta READ beta WRITE setBeta NOTIFY betaChanged)
    Q_PROPERTY(double gamma READ gamma WRITE setGamma NOTIFY gammaChanged)
    Q_PROPERTY(bool processingEnabled READ processingEnabled WRITE setProcessingEnabled NOTIFY processingEnabledChanged)

public:
    ImageItem(QQuickPaintedItem* parent = nullptr);
    void paint(QPainter *painter);
    QImage image() const;
    void setImage(const QImage& image);
    double alpha() const {return _alpha;}
    void setAlpha(const double alpha);
    double beta() const {return _beta;}
    void setBeta(const double beta);
    double gamma() const {return _gamma;}
    void setGamma(const double gamma);
    bool processingEnabled() const {return _processingEnabled;}
    void setProcessingEnabled(const bool processingEnabled);
    Q_INVOKABLE QString fileNmae() const;

public slots:
    void readImage();
    void clearImage();
    void terminate();

private slots:
    void handleResults(const QImage& image);

signals:
    void imageChanged(const QImage& image);
    void alphaChanged(const int alpha);
    void betaChanged(const double beta);
    void gammaChanged(const double gamma);
    void processingEnabledChanged(const bool processingEnabled);
    void imageCleared();
    void stopWorker();

private:
    ImageWorkerThread _workerThread;
    QString _fileName;
    QImage _currentImage;
    bool _processingEnabled;
    double _alpha = 1;
    double _beta = 1;
    double _gamma = 1;
};
