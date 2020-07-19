#ifndef PLAYER_H
#define PLAYER_H

#include <sndfile.h>
#include <portaudio.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

typedef enum player_state {
    PLAYING,
    PAUSED
} player_state;

typedef struct {
    SNDFILE *file;
    SF_INFO info;
    PaStream *stream;
    float volume;
    player_state state;
} Player;

Player *Player_init(const char *file_name, PaStreamCallback cb);
void Player_start(Player *player);
void Player_pause(Player *player);
void Player_stop(Player *player);
void Player_free(Player *player);

#endif