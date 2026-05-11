---
id: porting
title: Porting Guide
---

# Porting Guide

iodoom is designed to be ported to any platform. You implement six C functions; the engine handles everything else.

## Overview

Create a file named `src/iodoom_yourplatform.c` and implement the functions below. Then add it to the `SRC_DOOM` list in `src/Makefile`.

```c
#include "iodoom.h"

void     DG_Init(void);
void     DG_DrawFrame(void);
void     DG_SleepMs(uint32_t ms);
uint32_t DG_GetTicksMs(void);
int      DG_GetKey(int *pressed, unsigned char *key);
void     DG_SetWindowTitle(const char *title); // optional
```

---

## Function reference

### `DG_Init`

Called once at startup. Create your window, framebuffer, and any hardware resources here.

```c
void DG_Init(void)
{
    // Open a 640×400 window (or IODOOM_RESX × IODOOM_RESY)
    // Allocate anything you need
}
```

The framebuffer `DG_ScreenBuffer` is already allocated by the engine before `DG_Init` is called.

---

### `DG_DrawFrame`

Called every frame. `DG_ScreenBuffer` contains the rendered frame as `IODOOM_RESX × IODOOM_RESY` 32-bit XRGB pixels (byte order: `0x00RRGGBB`).

```c
void DG_DrawFrame(void)
{
    // 1. Blit DG_ScreenBuffer to your display
    // 2. Poll and enqueue input events
    // 3. Present / flip
}
```

:::caution
Do not block in `DG_DrawFrame`. Input events must be polled here to avoid the OS marking the window as unresponsive.
:::

---

### `DG_SleepMs`

Sleep for at least `ms` milliseconds. Exact precision is not required.

```c
void DG_SleepMs(uint32_t ms)
{
    usleep(ms * 1000);  // POSIX example
}
```

---

### `DG_GetTicksMs`

Return milliseconds elapsed since program start. Must be monotonically increasing.

```c
uint32_t DG_GetTicksMs(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
```

---

### `DG_GetKey`

Dequeue one keyboard event. Return `1` if an event was available, `0` if the queue is empty.

```c
int DG_GetKey(int *pressed, unsigned char *key)
{
    if (queue_empty()) return 0;
    event_t e = dequeue();
    *pressed = e.down;  // 1 = key down, 0 = key up
    *key     = e.doom_key;
    return 1;
}
```

Key codes are defined in `doomkeys.h`. Map your platform's key codes with a switch statement — see `src/iodoom_sdl.c` for a complete example.

---

### `DG_SetWindowTitle` *(optional)*

Doom calls this to set the window title from the WAD file name. Safe to leave empty.

```c
void DG_SetWindowTitle(const char *title)
{
    // set_window_title(title);
}
```

---

## Main loop

```c
int main(int argc, char **argv)
{
    iodoom_Create(argc, argv);  // init + D_DoomMain

    for (;;) {
        iodoom_Tick();          // one game tick
    }

    return 0;
}
```

---

## Key code mapping

Common mappings from `doomkeys.h`:

| Doom key constant | Typical mapping |
|---|---|
| `KEY_ENTER` | Return / Enter |
| `KEY_ESCAPE` | Escape |
| `KEY_LEFTARROW` | Left arrow |
| `KEY_RIGHTARROW` | Right arrow |
| `KEY_UPARROW` | Up arrow |
| `KEY_DOWNARROW` | Down arrow |
| `KEY_FIRE` | Left Ctrl |
| `KEY_USE` | Space |
| `KEY_RSHIFT` | Left/Right Shift |
| `KEY_F1`–`KEY_F12` | Function keys |

For printable ASCII keys, pass the lowercase ASCII value directly.

---

## Minimal example

See `src/iodoom_sdl.c` for a complete, well-commented reference implementation including:

- Integer-scaled rendering with aspect correction
- Full key mapping
- Mouse look
- Software FPS cap with high-resolution spin loop
- Runtime fullscreen and mouse grab toggle
