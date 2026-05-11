---
id: configuration
title: Configuration
---

# Configuration

iodoom is configured entirely through **environment variables** — no config files, no recompile.

## Environment variables

| Variable | Default | Description |
|---|---|---|
| `IODOOM_SCALE` | `0` | Window scale. `0` = auto-fit, `1`/`2`/`3` = integer scale |
| `IODOOM_FPS` | `35` | FPS cap. `35` matches Doom's native game rate |
| `IODOOM_VSYNC` | `0` | `1` to enable VSync (may override FPS cap) |
| `IODOOM_FULLSCREEN` | `0` | `1` to launch in fullscreen |
| `IODOOM_MOUSE` | `0` | `1` to enable relative mouse (mouse look) |

## Examples

```bash
# Pixel-perfect 2× window, VSync on
IODOOM_SCALE=2 IODOOM_VSYNC=1 ./iodoom -iwad doom.wad

# Fullscreen with mouse look
IODOOM_FULLSCREEN=1 IODOOM_MOUSE=1 ./iodoom -iwad doom.wad

# Native FPS with VSync (smooth on 60 Hz monitors)
IODOOM_FPS=200 IODOOM_VSYNC=1 ./iodoom -iwad doom.wad

# Strict native Doom behaviour
IODOOM_FPS=35 IODOOM_SCALE=1 ./iodoom -iwad doom.wad
```

## Scaling modes

### Auto-fit (`IODOOM_SCALE=0`)
The window starts at 2× and is freely resizable. The renderer integer-snaps to the largest multiple of 640×400 that fits, with letterboxing. Pixels remain crisp.

### Integer scale (`IODOOM_SCALE=1|2|3`)
The window is locked to exactly N × 640×400. Guarantees perfect pixel alignment with no sub-pixel blurring.

## FPS cap

Doom's game logic runs at 35 Hz regardless of rendering rate. The `IODOOM_FPS` cap only controls how often frames are *drawn*.

- `35` — one render per game tick. Lowest CPU use, matches original feel.
- `70`/`140` — smoother for displays that interpolate input.
- `0` — uncapped (not recommended; busy-loops the CPU).

The cap uses a high-resolution performance counter with a sub-millisecond spin at the end for accuracy.

## Runtime controls

These work while the game is running:

| Key | Action |
|---|---|
| `F11` | Toggle fullscreen |
| `` ` `` (backtick) | Toggle mouse grab on/off |
