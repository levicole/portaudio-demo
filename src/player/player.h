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

Player *init_Player(const char *file_name, PaStreamCallback cb);
void start_Player(Player *player);
void stop_Player(Player *player);
void free_Player(Player *player);

#endif