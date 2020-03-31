#pragma once

#include <QQuickImageProvider>
#include <QPixmap>
#include <QCoreApplication>

class VideoWorkerThread;

class VideoProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(QImage::Format format READ format WRITE setFormat NOTIFY formatChanged)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(QString resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)

public:
    VideoProvider(QCoreApplication* app, QObject* parent = nullptr);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
    bool running() const {return _running;}
    void setRunning(const bool running);
    QImage::Format format() const {return _format;}
    void setFormat(const QImage::Format format);
    int fps() const {return _fps;}
    void setFps(const int fps);
    QString resolution() const {return _resolution;}
    void setResolution(const QString& resolution);
    int brightness() const {return _brightness;}
    void setBrightness(const int brightness);
    int contrast() const {return _contrast;}
    void setContrast(const int contrast);
    int saturation() const {return _saturation;}
    void setSaturation(const int saturation);
    int exposure() const {return _exposure;}
    void setExposure(const int exposure);

    Q_INVOKABLE QString getCameraName() const;

public slots:
    void initiate();
    void terminate();
    void handleResults(const QImage& image);

signals:
    void runningChanged(const bool running);
    void formatChanged(const QImage::Format format);
    void fpsChanged(const int fps);
    void resolutionChanged(const QString& resolution);
    void brightnessChanged(const int brightness);
    void contrastChanged(const int contrast);
    void saturationChanged(const int saturation);
    void exposureChanged(const int exposure);
    void stopWorker();

private:
    void createWorkerConnections();

    QCoreApplication* _app; // added to call processEvents(), should replace frame number count
    VideoWorkerThread* _workerThread;
    QImage frameImage{};
    QPixmap pixmap{};
    bool _running = false;
    QImage::Format _format = QImage::Format_RGB888;
    QString _resolution;
    int _fps = 30;
    int _brightness;
    int _contrast;
    int _saturation;
    int _exposure;
};
