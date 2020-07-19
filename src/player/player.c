#include "player.h"
#include <stdlib.h>

void _die(Player *player, const char *message);

Player *Player_init(const char *file_name, PaStreamCallback cb)
{
    Player *player;
    if((player = malloc(sizeof(Player))) == NULL) return NULL;

    SNDFILE *soundFile;
    PaError err;

    player->volume = 1.0f;
    player->state  = PAUSED;

    soundFile = sf_open(file_name, SFM_READ, &player->info);
    player->file = soundFile;

    err = Pa_Initialize();
    if (err != paNoError) _die(player, Pa_GetErrorText(err));

    err = Pa_OpenDefaultStream(&player->stream,
                               0,
                               2,
                               paFloat32,
                               SAMPLE_RATE,
                               256,
                               cb,
                               player);

    if (err != paNoError) _die(player, Pa_GetErrorText(err));
    
    return player;
};

void Player_start(Player *player) {
    PaError err;
    err = Pa_StartStream(player->stream);
    if (err != paNoError) {
        printf("Error starting the player: %s\r\n", Pa_GetErrorText(err));
        Player_free(player);
        exit(1);
    }
    player->state = PLAYING;
}

void Player_pause(Player *player) {
    PaError err;
    err = Pa_StopStream(player->stream);
    if (err != paNoError) _die(player, Pa_GetErrorText(err));
    player->state = PAUSED;
}

void Player_stop(Player *player) {
    PaError err;
    err = Pa_StopStream(player->stream);
    if (err != paNoError) _die(player, Pa_GetErrorText(err));
    err = Pa_CloseStream(player->stream);
    if (err != paNoError) _die(player, Pa_GetErrorText(err));
    err = Pa_Terminate();
    if (err != paNoError) _die(player, Pa_GetErrorText(err));
}

void Player_free(Player *player)
{
    sf_close(player->file);
    free(player);
};

void _die(Player *player, const char *message)
{
    printf("Error: %s\r\n", message);
    Player_free(player);
    exit(1);
}