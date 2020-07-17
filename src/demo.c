#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "player/player.h"

#define SAMPLE_RATE 44100
#define NUM_SECONDS 3

static int player_cb( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData)
{
    float *out;
    int sample_count;
    int remaining_samples;
    Player *data = (Player*)userData;
    out = (float*)outputBuffer;
    (void) inputBuffer;
    // this doesn't seem necessary but I guess it's a good idea to zero it out?
    memset(out, 0, sizeof(float) * framesPerBuffer * data->info.channels);
    sample_count = sf_read_float(data->file, out, framesPerBuffer * data->info.channels);
    remaining_samples = (framesPerBuffer * data->info.channels) - sample_count;
    if (remaining_samples > 0) {
        sf_seek(data->file, 0, SEEK_SET);
        sf_read_float(data->file, (out + sample_count), remaining_samples);
    }
    return 0;
}


int main(void)
{
    Player *player;

    player = init_Player("./rhodes.wav", player_cb);
    if (player == NULL) {
        printf("error initializing audio player");
        exit(1);
    }

    start_Player(player);

    // block main thread so the audio player continues to loop.
    char foo[10];
    gets(foo);

    stop_Player(player);
    free_Player(player);
    return 0;
}