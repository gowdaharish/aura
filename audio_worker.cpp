#include "audio_worker.h"
#include "fftw3.h"
#include "math.h"

#include <QDebug>

using namespace std::chrono_literals;

// callback function for audio output
int callback(const void*,
             void* output,
             unsigned long frameCount,
             const PaStreamCallbackTimeInfo*,
             PaStreamCallbackFlags,
             void *userData)
{
    OurData* data = static_cast<OurData*>(userData);
    int* cursor; // current pointer into the output
    int *out = static_cast<int*>(output);
    int thisSize = frameCount;
    int thisRead;

    cursor = out; // set the output cursor to the beginning
    while (thisSize > 0)
    {
        qDebug() << thisSize << endl;
        // seek to our current file position
        sf_seek(data->sndFile, data->position, SEEK_SET);

        // are we going to read past the end of the file?
        if (thisSize > (data->sfInfo.frames - data->position))
        {
            // if we are, only read to the end of the file
            thisRead = data->sfInfo.frames - data->position;
            // and then loop to the beginning of the file
            data->position = 0;
        }
        else
        {
            // otherwise, we'll just fill up the rest of the output buffer
            thisRead = thisSize;
            // and increment the file position
            data->position += thisRead;
        }

        // since our output format and channel interleaving is the same as
        // sf_readf_int's requirements
        // we'll just read straight into the output buffer
        sf_readf_int(data->sndFile, cursor, thisRead);

        // apply DSP on cursor and replace the origitnal cursor

        // increment the output cursor
        cursor += thisRead;
        // decrement the number of samples left to process
        thisSize -= thisRead;
    }

    return paContinue;
}

AudioWorker::AudioWorker(QObject* parent) : QThread{parent}
{
}

AudioWorker::~AudioWorker()
{
    quit();
    wait();
}

void AudioWorker::run()
{
    while(!_stop)
    {
        if (_playing)
            playAudio();
    }
}

void AudioWorker::loadFile(const QString& filePath)
{
    _data = (OurData *)malloc(sizeof(OurData));

    //initialize our data
    _data->position = 0;
    _data->sfInfo.format = 0;
    // try to open the file
    _data->sndFile = sf_open(filePath.toStdString().c_str(), SFM_READ, &_data->sfInfo);

    const auto numFrames = _data->sfInfo.frames;
    int soundArray[numFrames];
    memset(soundArray, 0, numFrames);
    sf_readf_int(_data->sndFile,  soundArray, numFrames);

    if (sizeof(soundArray) > 0)
        emit writeToCanvas(soundArray);

        emit fileLoaded(_data->sfInfo.format, _data->sfInfo.channels, _data->sfInfo.samplerate, _data->sfInfo.frames);
  }

void AudioWorker::initializePortAudio()
{
    // make sure to terminate previous active treams
    Pa_Terminate();

    // start portaudio
    Pa_Initialize();

    // set the output parameters
    _outputParameters.device = Pa_GetDefaultOutputDevice();
    _outputParameters.channelCount = _data->sfInfo.channels;
    _outputParameters.sampleFormat = paInt32;
    // 200 ms ought to satisfy even the worst sound card
    _outputParameters.suggestedLatency = 0.2;
    _outputParameters.hostApiSpecificStreamInfo = 0;



    // try to open the output
    error = Pa_OpenStream(&stream,
                          0, // no input
                          &_outputParameters,
                          _data->sfInfo.samplerate,
                          paFramesPerBufferUnspecified, // let portaudio choose the buffersize
                          paNoFlag, // no special modes (clip off, dither off)
                          callback, // callback function defined above
                          _data ); /* pass in our data structure so the
    callback knows what's up */

    // if we can't open it, then bail out
    if (error)
    {
        printf("error opening output, error code = %i\n", error);
        Pa_Terminate();
        return;
    }
}

void AudioWorker::terminatePortAudio()
{
//    if (auto result = sf_close(_data->sndFile); result != 0)
//        qDebug() << "couldn't close the SNDFILE, an error occured";
    Pa_Terminate();
}

int AudioWorker::playAudio()
{
    qDebug() << "play called" << endl;
    initializePortAudio();
    // when we start the stream, the callback starts getting called
    Pa_StartStream(stream);
    Pa_Sleep(2000); /* pause for 2 seconds (2000ms) so we can hear a bit of the output */
    Pa_StopStream(stream); // stop the stream

    return 0;
}

void AudioWorker::stopAudio()
{
    qDebug() << "stopping audio plyaback: " << _playing << endl;
    Pa_StopStream(stream);
}

void AudioWorker::setPlaying(const bool playing)
{
    qDebug() << "playing is being set:" << playing << endl;
    _playing = playing;
}

void AudioWorker::setStop(const bool stop)
{
    qDebug() << "stop playing" << endl;
    _stop = stop;
}
