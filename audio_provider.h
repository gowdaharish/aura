#pragma once

#include <QObject>
#include <QString>
#include <QThread>
#include <QVector>

class AudioWorker;

class AudioProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector<int> canvasData READ canvasData NOTIFY canvasDataChanged)

public:
    AudioProvider(QObject* parent = nullptr);
    QVector<int> canvasData() const {return _canvasData;}
    Q_INVOKABLE QString filePath() const {return _fileName;};

public slots:
    void getSupportedCodes();
    void loadAudioFile();
    void playAudioFile();
    void stopAudioFile();
    void handleCanvasData(const int data[]);
    void storeFileData(double format, double channels, double sampleRate, double frames);

signals:
    void fileNameChanged(const QString& filePath);
    void canvasDataChanged(const QVector<int> canvasData);
    void fileInfoUpdated(const int fileFormat, const int fileChannels, const int fileSampleRate, const int fileFramesNum);

private:
    QString _fileName;
    AudioWorker* _worker;
    QVector<int> _canvasData;
    int _format, _channels, _sampleRate, _frames;
};
