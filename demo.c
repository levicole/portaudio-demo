#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sndfile.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define NUM_SECONDS 3

typedef struct {
    SNDFILE *file;
    SF_INFO info;
} callback_data;

static int patestCallBack( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    float *out;
    int sample_count;
    callback_data *data = (callback_data*)userData;
    out = (float*)outputBuffer;
    (void) inputBuffer;

    // this doesn't seem necessary but I guess it's a good idea to zero it out?
    memset(out, 0, sizeof(float) * framesPerBuffer * data->info.channels);
    sample_count = sf_read_float(data->file, out, framesPerBuffer * data->info.channels);
    if (sample_count < framesPerBuffer) {
        return paComplete;
    }
    return 0;
}


int main(void)
{
    SNDFILE *soundFile;
    PaError err;
    PaStream *stream;
    callback_data data;

    soundFile = sf_open("./rhodes.wav", SFM_READ, &data.info);
    printf("channels: %d, samplerate: %d\r\n", data.info.channels, data.info.samplerate);
    data.file = soundFile;

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, patestCallBack, &data);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    while((err=Pa_IsStreamActive(stream)) == 1)
    {
        Pa_Sleep(100);
    }

    Pa_StopStream(stream);
    if (err != paNoError) goto error;

    Pa_CloseStream(stream);
    if (err != paNoError) goto error;

error:
    err = Pa_Terminate();
    if (err != paNoError)
    {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
    }
    
    sf_close(soundFile);

    return 0;
}