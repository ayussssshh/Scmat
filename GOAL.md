# Project Goal

## Goal

Build a small 3D rendering engine in **C** for **visualizing mathematical objects**.

The engine should make it easy to type in a math object (a function, a shape) and see it on screen, rotate it, and eventually record it.

## Current Phase

**Phase 1: get the core working.** Everything else builds on it, so we do not start the later objectives until the core is solid.

## Objectives

### 1. Core: surface rendering of `z = f(x,y)` (priority)

- Evaluate a function `f(x, y)` over a grid of points.
- Turn the grid into a 3D mesh (vertices + triangles).
- Render the mesh with a basic camera (rotate, zoom) and simple shading.
- Done when: `z = sin(x) * cos(y)` shows up as a rotatable surface.

### 2. 2D and 3D geometry shapes

- Shapes such as line, circle, polygon, cube, sphere, cylinder.
- Built **on top of the core** (same mesh + renderer), not a separate system.
- Done when: a cube and a sphere can be drawn in the same scene as a surface.

### 3. Video loop (to be defined later)

- Idea: animate a scene (e.g. a parameter `t` changing, or a rotating camera) and export it as a looping video or GIF.
- Details are intentionally left open for now.
- Only constraint for now: the core should be able to render a frame from a given state (time `t`), so animation is possible later.

### 4. High-level language binding (e.g. Python)

- Expose the engine as a C library with a small, clean API.
- Python wraps it (for example with `ctypes` or `cffi`).
- Target usage, roughly:

```python
import mathengine as me

scene = me.Scene()
scene.add_surface(lambda x, y: x**2 - y**2, x=(-2, 2), y=(-2, 2))
scene.show()
```

## Design Principles

- **Keep it simple.** Small, readable C. No big dependencies unless needed.
- **Core first.** Shapes, video, and Python all reuse the core.
- **Clean C API from the start**, so the Python binding is easy later.

## Out of Scope (for now)

- Fancy lighting, textures, or physically based rendering
- GUI / editor
- Parsing function strings like `"sin(x)*y"` (can come later; first version can take a C function pointer)

## Suggested Next Steps

1. Pick the output method (e.g. window via SDL2/OpenGL, or software renderer writing to an image).
2. Implement vector/matrix math (vec3, mat4).
3. Implement grid sampling for `f(x, y)` and mesh creation.
4. Render the first surface.