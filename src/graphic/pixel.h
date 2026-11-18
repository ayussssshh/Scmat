#pragma once

#include <stdint.h>

/*
 * Pixel: one 32-bit value, packed as 0xAARRGGBB (alpha in the top byte).
 *
 * This must match the SDL texture format chosen in window.c
 * (SDL_PIXELFORMAT_ARGB8888). If you change the layout here, change it there.
 *
 * Alpha should always be 0xFF for now: pixel_rgb() and the named colors
 * guarantee it. Clear the framebuffer with PIXEL_BLACK, not 0.
 */
typedef uint32_t Pixel;

/* Bit position of each channel inside the packed value. */
#define PIXEL_SHIFT_A 24
#define PIXEL_SHIFT_R 16
#define PIXEL_SHIFT_G 8
#define PIXEL_SHIFT_B 0

/* ---- Packing ---------------------------------------------------------- */

static inline Pixel pixel_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((Pixel)a << PIXEL_SHIFT_A) |
           ((Pixel)r << PIXEL_SHIFT_R) |
           ((Pixel)g << PIXEL_SHIFT_G) |
           ((Pixel)b << PIXEL_SHIFT_B);
}

static inline Pixel pixel_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return pixel_rgba(r, g, b, 0xFF);
}

/* ---- Unpacking -------------------------------------------------------- */

static inline uint8_t pixel_a(Pixel p) { return (uint8_t)((p >> PIXEL_SHIFT_A) & 0xFF); }
static inline uint8_t pixel_r(Pixel p) { return (uint8_t)((p >> PIXEL_SHIFT_R) & 0xFF); }
static inline uint8_t pixel_g(Pixel p) { return (uint8_t)((p >> PIXEL_SHIFT_G) & 0xFF); }
static inline uint8_t pixel_b(Pixel p) { return (uint8_t)((p >> PIXEL_SHIFT_B) & 0xFF); }

/* ---- Float conversion ------------------------------------------------- */

/* Clamp to [0, 1]. Also maps NaN to 0 (NaN fails both comparisons below). */
static inline float pixel_clamp01(float v)
{
    if (v > 0.0f) return (v < 1.0f) ? v : 1.0f;
    return 0.0f;
}

/* The one place float colors (0.0 to 1.0) become bytes. Rounds to nearest. */
static inline Pixel pixel_from_float(float r, float g, float b)
{
    return pixel_rgb((uint8_t)(pixel_clamp01(r) * 255.0f + 0.5f),
                     (uint8_t)(pixel_clamp01(g) * 255.0f + 0.5f),
                     (uint8_t)(pixel_clamp01(b) * 255.0f + 0.5f));
}

/* ---- Named colors (all opaque) ---------------------------------------- */

#define PIXEL_BLACK ((Pixel)0xFF000000u)
#define PIXEL_WHITE ((Pixel)0xFFFFFFFFu)
#define PIXEL_RED   ((Pixel)0xFFFF0000u)
#define PIXEL_GREEN ((Pixel)0xFF00FF00u)
#define PIXEL_BLUE  ((Pixel)0xFF0000FFu)