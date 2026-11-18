#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "graphic/pixel.h"
#include "graphic/window.h"

#define FB_W 800
#define FB_H 600

int main(void)
{
    Window *win = window_create("scmat", FB_W, FB_H);
    if (!win) return 1;

    Pixel *buf = malloc((size_t)FB_W * FB_H * sizeof(Pixel));
    if (!buf) {
        fprintf(stderr, "out of memory\n");
        window_destroy(win);
        return 1;
    }

    unsigned frame = 0;

    while (window_poll_events(win)) {
        /* Red grows left to right, green grows top to bottom,
         * blue slowly pulses with the frame counter. */
        uint8_t blue = (uint8_t)(frame & 0xFF);

        for (int y = 0; y < FB_H; y++) {
            for (int x = 0; x < FB_W; x++) {
                uint8_t r = (uint8_t)(x * 255 / (FB_W - 1));
                uint8_t g = (uint8_t)(y * 255 / (FB_H - 1));
                buf[y * FB_W + x] = pixel_rgb(r, g, blue);
            }
        }

        if (!window_present(win, buf, FB_W, FB_H, FB_W * (int)sizeof(Pixel)))
            break;

        frame++;
    }

    free(buf);
    window_destroy(win);
    return 0;
}