#include "audio_provider.h"
#include "audio_worker.h"

#include <QDebug>
#include <QFileDialog>
#include <QThread>

AudioProvider::AudioProvider(QObject* parent) : QObject{parent}
{
    _worker.moveToThread(&_workerThread);

    connect(&_workerThread, &QThread::finished, &_worker, &AudioWorker::deleteLater);
    connect(&_worker, &AudioWorker::fileLoaded, this, &AudioProvider::storeFileData);
    connect(&_worker, &AudioWorker::writeToCanvas, this, &AudioProvider::handleCanvasData);
    connect(this, &AudioProvider::fileNameChanged, &_worker, &AudioWorker::loadFile);
    connect(this, &AudioProvider::play, &_worker, &AudioWorker::playAudio);
    connect(this, &AudioProvider::stop, &_worker, &AudioWorker::stopAudio, Qt::DirectConnection);

    _workerThread.start();
}

AudioProvider::~AudioProvider()
{
    _worker.stopAudio();
    disconnect(this, 0, &_worker, 0);
    _workerThread.quit();
    _workerThread.wait();
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

    emit fileNameChanged(_fileName);
}

void AudioProvider::playAudioFile()
{
    qDebug() << "playing audio" << endl;
    emit play();
}

void AudioProvider::stopAudioFile()
{
    qDebug() << "stopping audio" << endl;
    emit stop();
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
