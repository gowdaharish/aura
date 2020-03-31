#include "audio_provider.h"
#include "audio_worker.h"

#include <QDebug>
#include <QFileDialog>
#include <QThread>

AudioProvider::AudioProvider(QObject* parent) : QObject{parent}
{
    qDebug() << "initializing" << endl;
    qDebug() << "AP thread: " << QThread::currentThreadId() << endl;

    _worker = new AudioWorker;
    connect(_worker, &AudioWorker::finished, _worker, &QObject::deleteLater);
    connect(_worker, &AudioWorker::fileLoaded, this, &AudioProvider::storeFileData);
    connect(this, &AudioProvider::fileNameChanged, _worker, &AudioWorker::loadFile);
    connect(_worker, &AudioWorker::writeToCanvas, this, &AudioProvider::handleCanvasData);
    _worker->start();
}

void AudioProvider::getSupportedCodes()
{
}

void AudioProvider::loadAudioFile()
{
    qDebug() << "AP thread: " << QThread::currentThreadId() << endl;
    _fileName = QFileDialog::getOpenFileName({}, tr("Open Image"), "/Users", tr("Image Files (*.wav *.ogg)"));

    if (_fileName.isEmpty())
    {
        qDebug() << "file is empty" << endl;
        return;
    }

    emit fileNameChanged(_fileName);
}

void AudioProvider::playAudioFile()
{
    _worker->setPlaying(true);
    qDebug() << "playing audio" << endl;
}

void AudioProvider::stopAudioFile()
{
    _worker->stopAudio();
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
