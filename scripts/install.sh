#!/usr/bin/env bash
# iodoom installer
# Usage: ./scripts/install.sh [--wad /path/to/doom.wad] [--prefix /usr/local]
set -euo pipefail

PREFIX="${PREFIX:-/usr/local}"
WAD=""

while [[ $# -gt 0 ]]; do
  case $1 in
    --wad)    WAD="$2";    shift 2 ;;
    --prefix) PREFIX="$2"; shift 2 ;;
    *) echo "Unknown option: $1"; exit 1 ;;
  esac
done

OS="$(uname -s)"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "==> iodoom installer"
echo "    OS     : $OS"
echo "    Prefix : $PREFIX"

# ── Install SDL2 ─────────────────────────────────────────────
install_deps() {
  case "$OS" in
    Linux)
      if command -v apt-get &>/dev/null; then
        echo "==> Installing SDL2 (apt)"
        sudo apt-get update -qq && sudo apt-get install -y libsdl2-dev build-essential
      elif command -v dnf &>/dev/null; then
        echo "==> Installing SDL2 (dnf)"
        sudo dnf install -y SDL2-devel gcc make
      elif command -v pacman &>/dev/null; then
        echo "==> Installing SDL2 (pacman)"
        sudo pacman -S --noconfirm sdl2 gcc make
      else
        echo "Please install SDL2 manually and re-run."
        exit 1
      fi
      ;;
    Darwin)
      if command -v brew &>/dev/null; then
        echo "==> Installing SDL2 (brew)"
        brew install sdl2
      else
        echo "Homebrew not found. Install it from https://brew.sh first."
        exit 1
      fi
      ;;
    *)
      echo "Unsupported OS: $OS"
      exit 1
      ;;
  esac
}

install_deps

# ── Build ─────────────────────────────────────────────────────
echo "==> Building iodoom (release)"
cd "$ROOT/src"
make clean
make sdl RELEASE=1 -j"$(nproc 2>/dev/null || sysctl -n hw.logicalcpu 2>/dev/null || echo 2)"

# ── Install binary ────────────────────────────────────────────
echo "==> Installing to $PREFIX/bin/iodoom"
sudo install -Dm755 "$ROOT/src/iodoom" "$PREFIX/bin/iodoom"

# ── WAD setup ─────────────────────────────────────────────────
WADS_DIR="$HOME/.local/share/iodoom"
mkdir -p "$WADS_DIR"

if [[ -n "$WAD" && -f "$WAD" ]]; then
  echo "==> Copying WAD to $WADS_DIR/"
  cp "$WAD" "$WADS_DIR/"
  WAD_NAME="$(basename "$WAD")"
  echo "==> Run: iodoom -iwad $WADS_DIR/$WAD_NAME"
else
  echo ""
  echo "No WAD provided. To play:"
  echo "  iodoom -iwad /path/to/doom.wad"
  echo ""
  echo "A free WAD (Doom 1 shareware) can be downloaded from:"
  echo "  https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad"
fi

echo ""
echo "Done! Run 'iodoom --help' to see options."
