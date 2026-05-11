---
id: wad-files
title: WAD Files
---

# WAD Files

Doom stores all game data — levels, graphics, sounds, music — in a **WAD** file. iodoom does not include any WAD; you must supply your own.

## Free WADs

### Doom 1 Shareware (`doom1.wad`)
The shareware episode is free and legal to distribute:

```bash
wget https://distro.ibiblio.org/slitaz/sources/packages/d/doom1.wad
./iodoom -iwad doom1.wad
```

### FreeDoom
A free, open-source complete Doom replacement:

```bash
# Download from https://github.com/freedoom/freedoom/releases
./iodoom -iwad freedoom1.wad
./iodoom -iwad freedoom2.wad
```

## Commercial WADs

If you own Doom on Steam or GOG, the WAD is in the install directory:

```
# Steam (Linux)
~/.steam/steam/steamapps/common/Ultimate Doom/base/DOOM.WAD
~/.steam/steam/steamapps/common/Doom 2/base/DOOM2.WAD

# Steam (macOS)
~/Library/Application Support/Steam/steamapps/common/...
```

## Using multiple WADs (mods)

```bash
# Load a PWad mod on top of the IWAD
./iodoom -iwad doom.wad -file mymod.wad

# Multiple PWads
./iodoom -iwad doom2.wad -file mod1.wad mod2.wad
```

## Supported IWADs

| File | Game |
|---|---|
| `doom1.wad` | Doom I Shareware |
| `doom.wad` | The Ultimate Doom |
| `doom2.wad` | Doom II: Hell on Earth |
| `tnt.wad` | Final Doom: TNT Evilution |
| `plutonia.wad` | Final Doom: The Plutonia Experiment |
| `freedoom1.wad` | FreeDoom Phase 1 |
| `freedoom2.wad` | FreeDoom Phase 2 |
