#pragma once

#include "portaudio.h"

#include <QObject>
#include <QThread>

class AudioWorker : public QThread
{
    Q_OBJECT

public:
    AudioWorker(QThread* parent = nullptr);
    ~AudioWorker();
    Q_INVOKABLE int playAudio();
    Q_INVOKABLE void stopAudio();

public slots:
    void loadFile(const QString& file);
    void setPlaying(const bool playing);

signals:
    void resultReady(const QString &result);
    void writeToCanvas(const int[]);
    void fileLoaded(double format, double channels, double sampleRate, double frames);

protected:
    void run() override;

private:
    PaStream* stream = nullptr;
    PaError error;
    bool _playing = false;
};
