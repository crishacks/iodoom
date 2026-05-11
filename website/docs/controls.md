---
id: controls
title: Controls
---

# Controls

## Default keyboard layout

| Key | Action |
|---|---|
| **Arrow keys** | Move / turn |
| **Ctrl** | Fire |
| **Space** | Open door / use switch |
| **Shift** | Run |
| **Alt + Arrow** | Strafe |
| **,** / **.** | Strafe left / right |
| **1–7** | Select weapon |
| **Tab** | Automap |
| **F1** | Help |
| **F2** | Save game |
| **F3** | Load game |
| **F5** | Detail toggle |
| **F6** | Quicksave |
| **F7** | End game |
| **F9** | Quickload |
| **F10** | Quit |
| **Esc** | Menu |
| **Pause** | Pause |
| **+** / **-** | Resize view |

## iodoom runtime keys

| Key | Action |
|---|---|
| **F11** | Toggle fullscreen |
| `` ` `` | Toggle mouse grab |

## Mouse look (`IODOOM_MOUSE=1`)

When mouse look is enabled:
- **Move mouse** — turn left/right
- **Left button** — Fire
- **Right button** — Use
- **Middle button** — Strafe

Enable with:
```bash
IODOOM_MOUSE=1 ./iodoom -iwad doom.wad
```

Press `` ` `` at any time to release / re-grab the cursor.
