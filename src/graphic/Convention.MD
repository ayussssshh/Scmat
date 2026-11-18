# graphic/ conventions

Decisions for the graphic layer. Read this before editing `window.c`,
`framebuffer.c` or `pixel.h`. If a decision changes, change it here first.

## Layout

```
graphic/
  CONVENTIONS.md   this file
  pixel.h          Pixel type + color helpers (header only)
  window.h/.c      SDL3 window, texture upload, input events
  framebuffer.h/.c pixel memory + drawing primitives
```

Dependency rule: `window.c` is the only file that includes SDL headers.
`window.h`, `framebuffer.h` and `pixel.h` never include SDL or any 3D/math header.

## 1. Pixel format

`Pixel` is `uint32_t`, packed `0xAARRGGBB`. Alpha is always `0xFF`.
Defined in `pixel.h`. The SDL texture must be `SDL_PIXELFORMAT_ARGB8888`.
Clear with `PIXEL_BLACK`, never `0`.

## 2. Framebuffer size

- Fixed-size framebuffer, default **800 x 600**.
- The window starts at the same size and is resizable.
- SDL scales the texture to the window (linear filter, aspect ratio kept via
  letterboxing). The framebuffer does **not** resize with the window.
- Framebuffer size is independent of `window_create`; `window_present`
  receives the framebuffer size on every call.

## 3. Numeric types

- `float` for all 3D and math data (never `double` in the engine's data).
- `int` for pixel coordinates, sizes, counts.
- `uint32_t` for pixels and mesh indices.
- Always include `<stdint.h>` and `<stdbool.h>`. C standard: C11.

## 4. Ownership

- `window.c` owns SDL objects only (window, renderer, texture).
- `framebuffer.c` owns pixel memory: `fb_create` allocates, `fb_destroy` frees.
- `window_present` only borrows the pixel pointer for the duration of the
  call and never stores it.
- Whoever creates something destroys it.

## 5. Errors

- Functions that can fail return `bool` (`true` = success).
- `*_create` functions return a pointer, `NULL` on failure.
- Log the error where it happens: `SDL_Log` + `SDL_GetError()` in `window.c`,
  `fprintf(stderr, ...)` everywhere else. Callers just check the result.
- Every `*_destroy` function accepts `NULL` and partially built objects.

## 6. Handles

`Window` is opaque: `typedef struct Window Window;` in `window.h`, the struct
body only in `window.c`. `Framebuffer` is a plain struct (the renderer needs to
write into `pixels` directly):

```c
typedef struct {
    Pixel *pixels;
    int    width;
    int    height;
    int    pitch;   /* bytes per row, normally width * 4 */
} Framebuffer;
```

## 7. Input

Plain struct in `window.h`, filled by `window_poll_events`.

- Persistent state (kept between frames): mouse x/y, button states, key states.
- Per-frame values (reset at the start of each poll): mouse dx/dy, wheel.
- `quit` flag.
- Own enums, no SDL types. Keys: `ESCAPE, UP, DOWN, LEFT, RIGHT, W, A, S, D,
  Q, E, SPACE`, plus a `COUNT` entry. Mouse buttons: `LEFT, MIDDLE, RIGHT`.
  Mapping from SDL keycodes happens inside `window.c`.

## 8. Naming

- Functions: `window_`, `fb_`, `pixel_` prefixes. Types: `Window`,
  `Framebuffer`, `Pixel`, `Input`. Enums/constants: `KEY_`, `MOUSE_`, `PIXEL_`.
- `#pragma once` in every header.
- Each `.c` includes its own header first.

## 9. Coordinates

- Framebuffer: origin top-left, x right, y **down**.
- 3D world: y **up**.
- The flip happens in exactly one place: the viewport transform in the
  projection step (NDC to pixel coordinates). Nowhere else.

## 10. Public vs internal

- In `window.h`: opaque `Window`, `Input`, the key/button enums, and
  `window_create`, `window_destroy`, `window_present`, `window_poll_events`,
  `window_get_size`, `window_set_title`.
- `static` inside `window.c`: SDL event to `Input` translation, key mapping,
  texture (re)creation.