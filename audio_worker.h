#pragma once

#include "portaudio.h"
#include "sndfile.h"

#include <QObject>
#include <QThread>

/* Data structure to pass to callback
includes the sound file, info about the sound file, and a position
cursor (where we are in the sound file) */
struct OurData
{
    SNDFILE* sndFile;
    SF_INFO sfInfo;
    int position;
};

class AudioWorker : public QThread
{
    Q_OBJECT

public:
    AudioWorker(QObject* parent = nullptr);
    ~AudioWorker();
    Q_INVOKABLE int playAudio();
    Q_INVOKABLE void stopAudio();

public slots:
    void loadFile(const QString& file);
    void setPlaying(const bool playing);
    void setStop(const bool stop);
    void terminatePortAudio();
    void initializePortAudio();

signals:
    void resultReady(const QString &result);
    void writeToCanvas(const int[]);
    void fileLoaded(double format, double channels, double sampleRate, double frames);

protected:
    void run() override;

private:
    OurData* _data = nullptr;
    PaStreamParameters _outputParameters;
    PaStream* stream = nullptr;
    PaError error;
    bool _playing = false;
    bool _stop;

};
