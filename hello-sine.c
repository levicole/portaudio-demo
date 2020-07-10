#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sndfile.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define NUM_SECONDS 3
#define NUM_SAMPLES (NUM_SECONDS * SAMPLE_RATE)
#define PI 3.141592625
#define FREQUENCY 440

void generate_sine(float *samples, int length);
float sine_sample(int sample);

void generate_sine(float *samples, int length)
{
    for (int j = 0; j < length; j++)
    {
        samples[j] =  (float) sine_sample(j);
    }
}

float sine_sample(int sample)
{
    return sin(2 * PI * FREQUENCY * sample / SAMPLE_RATE);
}

typedef struct
{
    float left_phase;
    float right_phase;
}
paTestData;

static int patestCallBack( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer;

    for (i = 0; i < framesPerBuffer; i++)
    {
        *out++ = data->left_phase;
        *out++ = data->right_phase;
        data->left_phase += 0.01f;
        if (data->left_phase >= 1.0f) data->left_phase -= 2.0f;

        data->right_phase += 0.03f;
        if (data->right_phase >= 1.0f) data->right_phase -= 2.0f;
    }
    return 0;
}


int main(void)
{
    SNDFILE *soundFile;
    SF_INFO info;
    PaError err;
    PaStream *stream;
    static paTestData data;

    memset (&info, 0, sizeof (info)) ;

    soundFile = sf_open("./rhodes.wav", SFM_READ, &info);
    printf("channels: %d, samplerate: %d\r\n", info.channels, info.samplerate);

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    err = Pa_OpenDefaultStream(&stream,
                               0,
                               2,
                               paFloat32,
                               SAMPLE_RATE,
                               256,
                               patestCallBack,
                               &data);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;
    Pa_Sleep(NUM_SECONDS * 1000);

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