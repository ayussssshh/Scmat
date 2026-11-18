#include "graphic/window.h"

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/* Texture format must match the Pixel layout in pixel.h (0xAARRGGBB). */
#define WINDOW_TEXTURE_FORMAT SDL_PIXELFORMAT_ARGB8888

struct Window {
    SDL_Window   *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture  *sdl_texture;
    int tex_w, tex_h;
};

Window *window_create(const char *title, int w, int h)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return NULL;
    }

    Window *win = calloc(1, sizeof(Window));
    if (!win) {
        fprintf(stderr, "window_create: out of memory\n");
        goto fail;
    }

    win->sdl_window = SDL_CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE);
    if (!win->sdl_window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        goto fail;
    }

    win->sdl_renderer = SDL_CreateRenderer(win->sdl_window, NULL);
    if (!win->sdl_renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        goto fail;
    }

    if (!SDL_SetRenderVSync(win->sdl_renderer, 1)) {
        /* Not fatal: just log it. */
        fprintf(stderr, "VSync not enabled: %s\n", SDL_GetError());
    }

    return win;

fail:
    window_destroy(win);   /* handles NULL and half-built structs */
    return NULL;
}

void window_destroy(Window *win)
{
    if (win) {
        if (win->sdl_texture)  SDL_DestroyTexture(win->sdl_texture);
        if (win->sdl_renderer) SDL_DestroyRenderer(win->sdl_renderer);
        if (win->sdl_window)   SDL_DestroyWindow(win->sdl_window);
        free(win);
    }
    SDL_Quit();
}

bool window_poll_events(Window *win)
{
    (void)win;   /* unused until input handling is added */

    bool running = true;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            running = false;
        } else if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) {
            running = false;
        }
    }
    return running;
}

/* (Re)creates the streaming texture if the framebuffer size changed. */
static bool ensure_texture(Window *win, int fb_w, int fb_h)
{
    if (win->sdl_texture && win->tex_w == fb_w && win->tex_h == fb_h)
        return true;

    if (win->sdl_texture) {
        SDL_DestroyTexture(win->sdl_texture);
        win->sdl_texture = NULL;
    }

    win->sdl_texture = SDL_CreateTexture(win->sdl_renderer,
                                         WINDOW_TEXTURE_FORMAT,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         fb_w, fb_h);
    if (!win->sdl_texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    win->tex_w = fb_w;
    win->tex_h = fb_h;

    SDL_SetTextureScaleMode(win->sdl_texture, SDL_SCALEMODE_LINEAR);

    /* Keep the framebuffer's aspect ratio when the window is resized. */
    SDL_SetRenderLogicalPresentation(win->sdl_renderer, fb_w, fb_h,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    return true;
}

bool window_present(Window *win, const Pixel *pixels,
                    int fb_w, int fb_h, int pitch)
{
    if (!win || !pixels || fb_w <= 0 || fb_h <= 0)
        return false;

    if (!ensure_texture(win, fb_w, fb_h))
        return false;

    if (!SDL_UpdateTexture(win->sdl_texture, NULL, pixels, pitch)) {
        fprintf(stderr, "SDL_UpdateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderClear(win->sdl_renderer);   /* black bars around letterbox */

    if (!SDL_RenderTexture(win->sdl_renderer, win->sdl_texture, NULL, NULL)) {
        fprintf(stderr, "SDL_RenderTexture failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderPresent(win->sdl_renderer);
    return true;
}

void window_get_size(const Window *win, int *w, int *h)
{
    if (w) *w = 0;
    if (h) *h = 0;
    if (win) SDL_GetWindowSize(win->sdl_window, w, h);
}

void window_set_title(Window *win, const char *text)
{
    if (win) SDL_SetWindowTitle(win->sdl_window, text);
}