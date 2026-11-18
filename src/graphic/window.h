#pragma once

#include <stdbool.h>
#include "graphic/pixel.h"

/* Opaque handle: the struct body lives only in window.c. */
typedef struct Window Window;

/* Returns NULL on failure (error is printed to stderr). */
Window *window_create(const char *title, int w, int h);

/* Safe to call with NULL. */
void window_destroy(Window *win);

/* Drains the event queue. Returns false when the user wants to quit. */
bool window_poll_events(Window *win);

/* Uploads pixels (borrowed, not stored) and shows them.
 * pitch = bytes per row, normally fb_w * sizeof(Pixel). */
bool window_present(Window *win, const Pixel *pixels,
                    int fb_w, int fb_h, int pitch);

void window_get_size(const Window *win, int *w, int *h);
void window_set_title(Window *win, const char *text);