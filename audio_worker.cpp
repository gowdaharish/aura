#include "audio_worker.h"
#include "sndfile.h"
#include "fftw3.h"
#include "math.h"

#include <QDebug>

using namespace std::chrono_literals;

/* Data structure to pass to callback
includes the sound file, info about the sound file, and a position
cursor (where we are in the sound file) */
struct OurData
{
    SNDFILE* sndFile;
    SF_INFO sfInfo;
    int position;
};

// callback function for audio output
int Callback(const void*,
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

AudioWorker::AudioWorker(QThread* parent) : QThread{parent}
{
    qDebug() << "initializing the worker object" << endl;
}

AudioWorker::~AudioWorker()
{
//    this->finished({});
    Pa_Terminate(); // and shut down portaudio
}

void AudioWorker::run()
{
    while(true)
    {
        if (_playing)
            playAudio();

        std::this_thread::sleep_for(100ms);
    }
}

void AudioWorker::loadFile(const QString& filePath)
{
    // make sure to terminate previous active treams
    Pa_Terminate();

    OurData* data = (OurData *)malloc(sizeof(OurData));
    PaStreamParameters outputParameters;

    //initialize our data
    data->position = 0;
    data->sfInfo.format = 0;
    // try to open the file
    data->sndFile = sf_open(filePath.toStdString().c_str(), SFM_READ, &data->sfInfo);

    const auto numFrames = data->sfInfo.frames;
    int pictureArray[numFrames];
    memset(pictureArray, 0, numFrames);
    sf_readf_int(data->sndFile,  pictureArray, numFrames);
    qDebug() << sizeof(pictureArray) << endl;
    if (sizeof(pictureArray) > 0)
        emit writeToCanvas(pictureArray);

    // start portaudio
    Pa_Initialize();

    // set the output parameters
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = data->sfInfo.channels;
    outputParameters.sampleFormat = paInt32;
    // 200 ms ought to satisfy even the worst sound card
    outputParameters.suggestedLatency = 0.2;
    outputParameters.hostApiSpecificStreamInfo = 0;



    // try to open the output
    error = Pa_OpenStream(&stream,
                          0, // no input
                          &outputParameters,
                          data->sfInfo.samplerate,
                          paFramesPerBufferUnspecified, // let portaudio choose the buffersize
                          paNoFlag, // no special modes (clip off, dither off)
                          Callback, // callback function defined above
                          data ); /* pass in our data structure so the
    callback knows what's up */

    // if we can't open it, then bail out
    if (error)
    {
        printf("error opening output, error code = %i\n", error);
        Pa_Terminate();
        return;
    }

    emit fileLoaded(outputParameters.sampleFormat, outputParameters.channelCount, data->sfInfo.samplerate, data->sfInfo.frames);

    return;
}

int AudioWorker::playAudio()
{
    qDebug() << "play called" << endl;

    // when we start the stream, the callback starts getting called
    Pa_StartStream(stream);
    Pa_Sleep(30000); /* pause for 2 seconds (2000ms) so we can hear a bit of the output */
    Pa_StopStream(stream); // stop the stream

    return 0;
}

void AudioWorker::stopAudio()
{
    setPlaying(false);
    qDebug() << "stopping audio plyaback: " << _playing << endl;
    Pa_StopStream(stream); // stop the stream
}

void AudioWorker::setPlaying(const bool playing)
{
    qDebug() << "playing is being set:" << playing << endl;
    _playing = playing;
}
