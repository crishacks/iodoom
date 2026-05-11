---
id: contributing
title: Contributing
---

# Contributing

Contributions are welcome. Here's how to get started.

## Workflow

```bash
# 1. Fork https://github.com/crishacks/iodoom
# 2. Clone your fork
git clone https://github.com/YOUR_USER/iodoom

# 3. Create a branch
git checkout -b feat/my-improvement

# 4. Build and test
cd src && make sdl -j$(nproc)
./iodoom -iwad doom1.wad

# 5. Push and open a PR
git push origin feat/my-improvement
```

## Guidelines

- **C99 only** — no compiler extensions, no platform-specific UB
- Keep the core engine files (`d_main.c`, `i_*.c`, etc.) minimal — changes there affect every platform
- New platform files go in `src/iodoom_yourplatform.c`
- Match the existing style: tabs for indentation in engine files, 4-space in iodoom-specific files
- All PRs must pass the GitHub Actions CI (Linux + macOS builds, CodeQL)

## Reporting bugs

Open an issue at [github.com/crishacks/iodoom/issues](https://github.com/crishacks/iodoom/issues). Include:

- OS and version
- `gcc --version` or `clang --version`
- SDL2 version (`sdl2-config --version`)
- WAD file used
- Steps to reproduce

## Adding a new platform

1. Create `src/iodoom_yourplatform.c` implementing the six callbacks
2. Add a `Makefile.yourplatform` following the pattern of `Makefile`
3. Add a build job to `.github/workflows/build.yml`
4. Document it in the [Porting Guide](./porting)
