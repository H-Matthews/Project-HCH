# Project HCH

A C++20 game engine and the game built on it. SFML for windowing/rendering, EnTT for the ECS, TOML++ for config.

Platforms: Linux (Ubuntu), macOS and Windows. More detail in the [wiki](https://github.com/H-Matthews/Project-HCH/wiki).

## Layout

| Path | What it is |
|---|---|
| `engine/` | The `engine` static library: `core/` (app, state stack, messaging, config, ECS base), `utility/` (logging, strings, time), `vendor/` (EnTT, TOML++) |
| `games/HCH/` | The game: `main.cpp`, game layer in `src/`, plus its own `configs/` and `assets/` |
| `sandbox/` | Minimal executable that links only the engine, for trying engine features |
| `tests/` | GoogleTest binaries (`Core/`, `Utility/`, `Application/`) |

The engine never includes game code; a CTest (`core_independence_check`) enforces it.

## Requirements
- Git
- CMake 3.30+
- A C++20 compiler: GCC/Clang on Linux, Apple Clang (Xcode Command Line Tools) on macOS, Visual Studio 2022 or 2025 on Windows

SFML and GoogleTest are downloaded by CMake. macOS and Windows need no extra system packages. On Ubuntu, install SFML's system dependencies first:

```bash
sudo apt update && sudo apt install \
    libxrandr-dev libxcursor-dev libxi-dev libudev-dev libflac-dev \
    libvorbis-dev libgl1-mesa-dev libegl1-mesa-dev libdrm-dev libgbm-dev libfreetype-dev
```

## Build and run

Run from the repo root. Presets are in `CMakePresets.json` (`cmake --list-presets` to see them).

**Linux / macOS** (same Unix Makefiles presets)
```bash
cmake --preset debug-unix_makefiles        # or release-unix_makefiles
cmake --build --preset debug-unix_makefiles

./build/Debug/games/HCH/Project-HCH        # the game
./build/Debug/sandbox/Sandbox              # the sandbox
```

**Windows** (generates a Visual Studio solution in `build/`)
```bash
cmake --preset windows-VS22                # or windows-VS25
cmake --build build --config Release
```
Executables land in `build/games/HCH/Release/` and `build/sandbox/Release/`.

## Tests
```bash
ctest --test-dir ./build/Debug --output-on-failure          # Linux / macOS
ctest --test-dir ./build -C Release --output-on-failure     # Windows
```
Each `.cc` file under `tests/` is its own test binary. New test files need a CMake re-configure.

## VS Code
Install the C/C++ and CMake Tools extensions. Pick a configure preset (`CMake: Select Configure Preset`), then press F5 and choose a launch config (HCH or Sandbox, Debug or Release). Each one builds its target first.

## Formatting
`scripts/format.sh` runs clang-format (version 18) over the sources.
