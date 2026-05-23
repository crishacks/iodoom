# iodoom

A user-friendly fork of [doomgeneric](https://github.com/ozkl/doomgeneric).

![Build](https://github.com/thetuxuser/iodoom/actions/workflows/build.yml/badge.svg)

## What's new vs doomgeneric

| Feature | doomgeneric | iodoom |
|---|---|---|
| Window scaling | Fixed 640×400 | Auto-fit or 1×/2×/3× integer scale |
| FPS cap | None | Configurable (default 35 Hz) |
| Key queue | 16 entries | 32 entries (no dropped inputs) |
| Mouse look | No | Optional (`IODOOM_MOUSE=1`) |
| Fullscreen toggle | No | F11 at runtime |
| VSync | No | `IODOOM_VSYNC=1` |
| CI (Linux + macOS) | No | ✅ |
| Release binaries | No | ✅ (auto on tag push) |
| Release build | No | `make RELEASE=1` (-O3 + LTO) |

## Quick start

```bash
git clone https://github.com/thetuxuser/iodoom && cd iodoom
./scripts/install.sh --wad /path/to/doom.wad
```

> **No WAD?** The Doom 1 shareware WAD is free:
> ```
> wget https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad
> ```

## Configuration

Set via environment variables – no recompile needed.

| Variable | Default | Description |
|---|---|---|
| `IODOOM_SCALE` | `0` | `0`=auto-fit, `1`/`2`/`3`=integer scale |
| `IODOOM_FPS` | `35` | FPS cap (35 = Doom's native rate) |
| `IODOOM_VSYNC` | `0` | `1` to enable VSync |
| `IODOOM_FULLSCREEN` | `0` | `1` to start fullscreen |
| `IODOOM_MOUSE` | `0` | `1` to enable mouse look |

```bash
IODOOM_SCALE=2 IODOOM_FULLSCREEN=1 IODOOM_MOUSE=1 ./iodoom -iwad doom.wad
```

## Runtime keys

| Key | Action |
|---|---|
| `F11` | Toggle fullscreen |
| `` ` `` | Toggle mouse grab |

## Building

```bash
# Linux
sudo apt install libsdl2-dev
cd src && make sdl -j$(nproc)

# macOS
brew install sdl2
cd src && make sdl -j$(sysctl -n hw.logicalcpu)

# Optimized release build
make sdl RELEASE=1 -j$(nproc)
```

## License

GPLv2 – inherited from Doom source. Based on [doomgeneric](https://github.com/ozkl/doomgeneric) by ozkl.
