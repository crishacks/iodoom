---
id: intro
title: Introduction
sidebar_label: Introduction
slug: /intro
---

# iodoom

**iodoom** is a user-friendly, fast Doom port based on [doomgeneric](https://github.com/ozkl/doomgeneric). It adds a proper SDL2 platform layer, runtime configuration, smarter scaling, and a full CI/CD pipeline — while staying just as easy to port to new platforms.

## What it is

iodoom provides a clean C API (`iodoom.h`) and a ready-to-use SDL2 implementation. You can drop it into any project and have Doom running in minutes. Porting to a new platform means implementing six well-documented functions.

## What's improved over doomgeneric

| Feature | doomgeneric | iodoom |
|---|---|---|
| Window scaling | Fixed 640×400 | Auto-fit or 1×/2×/3× integer scale |
| FPS cap | None | Configurable, high-precision |
| Key queue | 16 entries | 32 entries |
| Mouse look | No | Optional (`IODOOM_MOUSE=1`) |
| Fullscreen toggle | No | F11 at runtime |
| VSync | No | `IODOOM_VSYNC=1` |
| CI (Linux + macOS) | No | ✅ |
| Release binaries | No | ✅ auto on tag |
| Build modes | One | Debug / Release (`RELEASE=1`) |

## Requirements

- **C99** compiler (GCC ≥ 9 or Clang ≥ 10)
- **SDL2** development headers (for the SDL2 platform)
- A **WAD file** — the actual Doom game data

:::tip Free WAD
The Doom 1 shareware WAD (`doom1.wad`) is free and legal:
```bash
wget https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad
```
:::

## Next steps

- [Building iodoom](./building) — compile from source
- [Configuration](./configuration) — env vars and runtime options
- [Porting Guide](./porting) — target a new platform
