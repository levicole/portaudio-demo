#include "gui.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_SDL_GL2_IMPLEMENTATION
#define NK_IMPLEMENTATION

#include "../includes/nuklear.h"
#include "../includes/nuklear_sdl_gl2.h"
#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 1200
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 800
#endif

void init_nuklear_gui(gui_cb cb, void *userData)
{
    SDL_Window *win;
    SDL_GLContext glContext;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;
    struct nk_colorf bg;

    /* SDL setup */
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    win = SDL_CreateWindow("Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    glContext = SDL_GL_CreateContext(win);
    SDL_GetWindowSize(win, &win_width, &win_height);

    /* GUI */

    ctx = nk_sdl_init(win);

    // Load fonts...this seems to be required.
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        nk_sdl_font_stash_end();
    }
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    while (running)
    {
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt))
        {
            if(evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }

        cb(ctx, userData);

        SDL_GetWindowSize(win, &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(bg.r, bg.g, bg.b, bg.a);
        nk_sdl_render(NK_ANTI_ALIASING_ON);
        SDL_GL_SwapWindow(win);
    }
    
cleanup:
    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(win);
    SDL_Quit();
}