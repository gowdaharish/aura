#include "audio_provider.h"
#include "audio_worker.h"

#include <QDebug>
#include <QFileDialog>
#include <QThread>

AudioProvider::AudioProvider(QObject* parent) : QObject{parent}
{
}

AudioProvider::~AudioProvider()
{
    if (!_worker)
        return;

    _worker->stopAudio();
}

void AudioProvider::getSupportedCodes()
{
    // return video codecs supported by the native platform
}

void AudioProvider::loadAudioFile()
{
    _fileName = QFileDialog::getOpenFileName({}, tr("Open Image"), "/Users", tr("Image Files (*.wav *.ogg)"));

    if (_fileName.isEmpty())
    {
        qDebug() << "file is empty" << endl;
        return;
    }

    if (_worker)
    {
        _worker->setStop(true);
        _worker = nullptr;
    }

    _worker = new AudioWorker{this};
    connect(_worker, &AudioWorker::finished, _worker, &AudioWorker::deleteLater);
    connect(_worker, &AudioWorker::fileLoaded, this, &AudioProvider::storeFileData);
    connect(_worker, &AudioWorker::writeToCanvas, this, &AudioProvider::handleCanvasData);
    connect(this, &AudioProvider::fileNameChanged, _worker, &AudioWorker::loadFile);
    connect(this, &AudioProvider::setPlaying, _worker, &AudioWorker::setPlaying);

    emit fileNameChanged(_fileName);
}

void AudioProvider::playAudioFile()
{
    qDebug() << "playing audio" << endl;
    emit setPlaying(true);
    _worker->setStop(false);
    _worker->start();
}

void AudioProvider::stopAudioFile()
{
    qDebug() << "stopping audio" << endl;
    _worker->setStop(true);
    _worker->stopAudio();
    _worker->terminatePortAudio();
}

void AudioProvider::handleCanvasData(const int data[])
{
    std::copy(data, data + 5000, std::back_inserter(_canvasData));
    emit canvasDataChanged(_canvasData);
}

void AudioProvider::storeFileData(double format, double channels, double sampleRate, double frames)
{
    qDebug() << format << channels << sampleRate << frames << endl;

    _format = format;
    _channels = channels;
    _sampleRate = sampleRate;
    _frames = frames;

    emit fileInfoUpdated(_format, _channels, _sampleRate, _frames);
}

void AudioProvider::terminate()
{
    emit setPlaying(false);
    _worker->setStop(true);
}
