---
id: api
title: API Reference
---

# API Reference

The iodoom public API is defined in `src/iodoom.h`.

## Resolution constants

```c
#define IODOOM_RESX 640   // override with -DIODOOM_RESX=N
#define IODOOM_RESY 400   // override with -DIODOOM_RESY=N
```

Override at compile time:
```bash
make sdl CFLAGS="-DIODOOM_RESX=1280 -DIODOOM_RESY=800"
```

## Pixel type

```c
typedef uint32_t pixel_t;  // XRGB, 0x00RRGGBB
```

With `-DCMAP256`:
```c
typedef uint8_t pixel_t;   // 8-bit palette index
```

## Framebuffer

```c
extern pixel_t *DG_ScreenBuffer;
```

Allocated by `iodoom_Create` before `DG_Init` is called. Size is `IODOOM_RESX × IODOOM_RESY × sizeof(pixel_t)` bytes. Written by the engine every frame; read by `DG_DrawFrame`.

---

## Engine functions

### `iodoom_Create`

```c
void iodoom_Create(int argc, char **argv);
```

Initialises the engine. Allocates `DG_ScreenBuffer`, calls `DG_Init()`, then calls `D_DoomMain()` which never returns. Must be called once before the main loop.

**Parameters:**
- `argc` / `argv` — pass your program's main arguments directly. Doom parses `-iwad`, `-file`, `-warp`, etc.

---

### `iodoom_Tick`

```c
void iodoom_Tick(void);
```

Runs one game tick: processes input, advances game logic, renders a frame, and calls `DG_DrawFrame`. Call in a tight loop after `iodoom_Create`.

:::note
`iodoom_Tick` is defined in `d_main.c`. The engine calls `DG_DrawFrame` inside it.
:::

---

## Platform callbacks

These are called by the engine. You implement them in your platform file.

### `DG_Init`
```c
void DG_Init(void);
```
Called once. Create your window and any hardware resources.

### `DG_DrawFrame`
```c
void DG_DrawFrame(void);
```
Called each frame. Blit `DG_ScreenBuffer`, poll events, present.

### `DG_SleepMs`
```c
void DG_SleepMs(uint32_t ms);
```
Sleep at least `ms` milliseconds.

### `DG_GetTicksMs`
```c
uint32_t DG_GetTicksMs(void);
```
Return monotonic millisecond counter since start.

### `DG_GetKey`
```c
int DG_GetKey(int *pressed, unsigned char *key);
```
Dequeue one event. Returns 1 if available, 0 if empty.
- `*pressed` — `1` for key-down, `0` for key-up
- `*key` — iodoom key code (see `doomkeys.h`)

### `DG_SetWindowTitle`
```c
void DG_SetWindowTitle(const char *title);
```
Optional. Set OS window title. Called with the WAD game description.

---

## Backwards-compatibility aliases

`iodoom.h` provides these for any code targeting the original doomgeneric API:

```c
#define DOOMGENERIC_RESX    IODOOM_RESX
#define DOOMGENERIC_RESY    IODOOM_RESY
#define doomgeneric_Create  iodoom_Create
#define doomgeneric_Tick    iodoom_Tick
```
