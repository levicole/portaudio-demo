#ifndef GUI_H
#define GUI_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
typedef void (*gui_cb)(struct nk_context *ctx, void *userData);
void init_nuklear_gui(gui_cb cb, void *userData);
#endif