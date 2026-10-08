# Sonic CD 2011 — Nintendo Wii U Port

An updated Nintendo Wii U port of the Sonic CD 2011 decompilation, based on **RSDKv3 1.3.3**.

## Project status

This is a community-maintained Wii U adaptation of the current RSDKv3 engine.

- Wii U compilation using devkitPro, WUT and CMake

- RPX and WUHB output

- SDL2 software rendering

- VSync enabled by default on Wii U

- Mod loader enabled in the documented build configuration

**Status:** Development build. Testing and compatibility improvements are ongoing.

## Requirements

- Nintendo Wii U capable of running homebrew

- SD card

- Your own legally obtained Sonic CD `Data.rsdk`

- For building: Windows with Docker Desktop, or a compatible devkitPro environment

Game data is not included.

## Building

From the repository root, run the following command in Windows PowerShell:

```powershell

docker run --rm -v "${PWD}:/project" -w /project devkitpro/devkitppc sh -c 'cmake -S . -B build-wiiu -G Ninja -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/cmake/WiiU.cmake -DPLATFORM=CafeOS -DRETRO_USE_HW_RENDER=OFF -DRETRO_SDL_VERSION=2 -DRETRO_MOD_LOADER=ON -DRETRO_DISABLE_PLUS=ON'

```

Then compile:

```powershell

docker run --rm -v "${PWD}:/project" -w /project devkitpro/devkitppc sh -c 'cmake --build build-wiiu --parallel 2'

```

The resulting application bundle is located at:

`build-wiiu/RSDKv3.wuhb`

## Installation

Copy the generated WUHB file to:

`SD:/wiiu/apps/SonicCD/RSDKv3.wuhb`

Place your own game data at:

`SD:/SonicCD/Data.rsdk`

The engine creates a `settings.ini` file when necessary. On Wii U, VSync defaults to enabled.

Preserve existing save files and settings when updating.

## Known limitations

- Some menus or features may not behave identically to the original commercial release.

- Intro video playback requires compatible video assets, which are not included.

- Additional testing is needed for compatibility and mod support.

## Credits

- **RSDKModding and contributors:** RSDKv3 decompilation

- **Clownacy, Rubberduckycooly, chuliRMG:** original Wii U port and associated contributions

- **Original Sonic CD developers and rights holders:** original game

This adaptation builds upon the work of the original projects. Refer to their repositories for licensing information and full contributor history.

## Upstream projects

- https://github.com/RSDKModding/RSDKv3-Decompilation

- https://github.com/Clownacy/Sonic-CD-11-Decompilation-Wii-U

This is an unofficial community project and is not affiliated with Sega.
