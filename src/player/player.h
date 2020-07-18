#ifndef PLAYER_H
#define PLAYER_H

#include <sndfile.h>
#include <portaudio.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

typedef struct {
    SNDFILE *file;
    SF_INFO info;
    PaStream *stream;
} Player;

Player *Player_init(const char *file_name, PaStreamCallback cb);
void Player_start(Player *player);
void Player_stop(Player *player);
void Player_free(Player *player);

#endif