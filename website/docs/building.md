---
id: building
title: Building
---

# Building iodoom

## One-command install

The fastest path on Linux or macOS:

```bash
git clone https://github.com/crishacks/iodoom && cd iodoom
./scripts/install.sh --wad /path/to/doom.wad
```

This installs SDL2, builds a release binary, and copies it to `/usr/local/bin/iodoom`.

---

## Manual build

### Linux (SDL2)

```bash
# Debian / Ubuntu
sudo apt install libsdl2-dev build-essential

# Fedora
sudo dnf install SDL2-devel gcc make

# Arch
sudo pacman -S sdl2 gcc make

cd src
make sdl -j$(nproc)
```

### macOS (SDL2)

```bash
brew install sdl2
cd src
make sdl -j$(sysctl -n hw.logicalcpu)
```

### Linux (Xlib, no SDL2)

```bash
sudo apt install libx11-dev build-essential
cd src
make xlib -j$(nproc)
```

---

## Build modes

| Command | Flags | Use for |
|---|---|---|
| `make sdl` | `-O2 -ggdb3` | Development, debugging |
| `make sdl RELEASE=1` | `-O3 -march=native -flto` | Distribution, benchmarking |
| `make sdl V=1` | (verbose) | Diagnosing build issues |

:::info Release builds
`RELEASE=1` enables link-time optimization and native CPU tuning. The resulting binary is noticeably faster but won't run on older CPUs than the one it was compiled on.
:::

---

## Running

```bash
./iodoom -iwad /path/to/doom.wad
```

Common WAD arguments:

```bash
./iodoom -iwad doom.wad              # Doom 1
./iodoom -iwad doom2.wad             # Doom 2
./iodoom -iwad doom.wad -file mod.wad # With a PWad mod
```

---

## Clean

```bash
cd src && make clean
```
