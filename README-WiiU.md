# Sonic CD 2011 - Nintendo Wii U Port

An unofficial Nintendo Wii U port of the Sonic CD 2011 decompilation, based on **RSDKv3 1.3.3**.

## Project status

**Public beta - community testing**

This port is under active development. The current experimental build has been tested on real Wii U hardware, including:

- Gameplay and stage transitions
- Japanese and US soundtracks
- Music switching, looping and sound effects
- Save and load functionality
- Wii U Pro Controller
- HOME menu suspension, resumption and normal exit
- Time travel and Special Stages
- Developer menu access

These tests do not guarantee compatibility with every setup or mod.

## Features

- Native Wii U homebrew application
- RPX and WUHB build support
- SDL2 software rendering
- VSync enabled by default
- Mod loader support
- Wii U controller shortcut for the developer menu
- Experimental audio stability improvements

## Requirements

- Nintendo Wii U capable of running homebrew
- SD card
- Your own legally obtained Sonic CD `Data.rsdk`
- For compiling: Docker Desktop with a Linux container environment, or a compatible devkitPro setup

**Game data and copyrighted assets are not included.**

## Installation

1. Copy `RSDKv3.wuhb` to `SD:/wiiu/apps/SonicCD/RSDKv3.wuhb`.
2. Place your legally obtained `Data.rsdk` at `SD:/SonicCD/Data.rsdk`.
3. Launch the application using your Wii U homebrew environment.

Back up existing save files and `settings.ini` before updating.

## Developer menu and mods

The build supports the RSDKv3 mod loader.

To enable the developer menu, set the following option in `SD:/SonicCD/settings.ini`:

```ini
[Dev]
DevMenu=true
```

On Wii U, press **L + R + SELECT (-)** on the same controller to open the developer menu.

Depending on the current game state and settings, the developer menu may also be accessible through the START/pause menu.

The MODS menu is available through the developer menu when enabled.

**Note:** Basic MODS menu navigation has been tested. Actual mod activation, deactivation and compatibility require further community testing.

## Building from source

From the repository root, configure the Wii U build:

```powershell
docker run --rm -v "${PWD}:/project" -w /project devkitpro/devkitppc sh -c 'cmake -S . -B build-wiiu -G Ninja -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/WiiU.cmake -DPLATFORM=CafeOS -DRETRO_USE_HW_RENDER=OFF -DRETRO_SDL_VERSION=2 -DRETRO_MOD_LOADER=ON -DRETRO_DISABLE_PLUS=ON'
```

Compile:

```powershell
docker run --rm -v "${PWD}:/project" -w /project devkitpro/devkitppc sh -c 'cmake --build build-wiiu --parallel 2'
```

Output:

`build-wiiu/RSDKv3.wuhb`

**Important:** Publicly distributed builds must have DLC/Plus disabled by default, as required by `LICENSE.md`. The Wii U CMake configuration enforces `RETRO_DISABLE_PLUS=ON`.

## Known limitations

- This is a beta release, not a final stable release.
- Mod compatibility has not been comprehensively tested.
- The audio protections have passed normal gameplay testing but have not been validated against every malformed Ogg file.
- Intro video playback requires compatible assets, which are not included.
- Some menus or features may differ from the original commercial release.

## Reporting bugs

Please open a GitHub Issue and include:

- Wii U hardware and controller used
- Game data version, if known
- Steps to reproduce the problem
- Expected and actual behavior
- Whether mods were enabled
- Screenshots or logs, if available

Do not upload copyrighted game data or proprietary assets.

## License and credits

This project follows the terms of `LICENSE.md`. Distribution is non-commercial, requires attribution, and must not include copyrighted game assets not supplied by the repository.

- **Christian "Taxman" Whitehead:** original RSDK engine
- **Rubberduckycooly and stxtic:** RSDK decompilation authors
- **RSDKModding and contributors:** upstream decompilation project
- **Clownacy, Rubberduckycooly and chuliRMG:** earlier Wii U port contributions
- **Original Sonic CD developers and rights holders:** original game

This is an unofficial community project and is not affiliated with Sega.

## Upstream projects

- https://github.com/RSDKModding/RSDKv3-Decompilation
- https://github.com/Clownacy/Sonic-CD-11-Decompilation-Wii-U