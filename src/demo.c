#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "player/player.h"
#include "gui/gui.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "includes/nuklear.h"

#define SAMPLE_RATE 44100
#define NUM_SECONDS 3

static nk_flags DEFAULT_WINDOW_MASK = NK_WINDOW_BORDER      |
                                      NK_WINDOW_MOVABLE     |
                                      NK_WINDOW_SCALABLE    |
                                      NK_WINDOW_MINIMIZABLE |
                                      NK_WINDOW_TITLE;

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
    for (unsigned long int i = 0; i < framesPerBuffer * data->info.channels; i++)
    {
        out[i] = out[i] * data->volume;
    }
    
    return 0;
}

void build_gui(struct nk_context *ctx, void *userData) {
    Player *player = (Player*)userData;

    if (nk_begin(ctx, "Audio Player", nk_rect(50,50,100,100), DEFAULT_WINDOW_MASK))
    {
        nk_layout_row_static(ctx, 20, 80, 1);
        nk_label(ctx, "Volume", NK_TEXT_LEFT);

        nk_layout_row_static(ctx, 20, 80, 1);
        nk_slider_float(ctx, 0, &player->volume, 1.0f, 0.01f);
    }

    nk_end(ctx);
}

int main(void)
{
    Player *player;

    player = Player_init("./rhodes.wav", player_cb);
    if (player == NULL) {
        printf("error initializing audio player");
        exit(1);
    }

    Player_start(player);

    // block main thread so the audio player continues to loop.
    init_nuklear_gui(build_gui, player);

    Player_stop(player);
    Player_free(player);
    return 0;
}