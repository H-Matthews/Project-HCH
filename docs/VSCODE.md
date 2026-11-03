# VS Code setup

How to build, run, debug and test from VS Code. The config lives in `.vscode/` and `CMakePresets.json`.

## One-time setup
When you open the folder, VS Code offers to install the extensions listed in `.vscode/extensions.json`. Accept, or run `Extensions: Show Recommended Extensions` later.
- **C/C++** (`ms-vscode.cpptools`) and **CMake Tools** (`ms-vscode.cmake-tools`) are required. The launch configs, tasks and presets don't work without them.
- **SonarLint** and **Even Better TOML** are optional. SonarLint reads `build/Debug/compile_commands.json`, and TOML is the format of the engine's config files.

On Linux/WSL you also need `gdb`.

## Pick your presets
CMake Tools drives everything from presets. Choose them from the status bar or the command palette:

| What | Command | Example (Linux) | Example (Windows) |
|---|---|---|---|
| Configure | `CMake: Select Configure Preset` | `debug-unix_makefiles` | `windows-VS22` |
| Build | `CMake: Select Build Preset` | `debug-unix_makefiles` | `windows-VS22` |
| Test | `CMake: Select Test Preset` | `debug-unix_makefiles` | `windows-VS22` |

Use matching names across the three. Linux has `debug-` and `release-` variants. Windows presets build Debug by default.

## Pick a target
Run `CMake: Select Launch/Debug Target`. The target can be the game (`Project-HCH`), the sandbox (`Sandbox`), or any test binary (one per `.cc` file under `tests/`). The launch configs and the build task below all act on this target.

## Build
- **Build the selected target:** run the `Build: selected target` task (`Tasks: Run Build Task`, or `Ctrl+Shift+B`).
- **Build everything:** `Tasks: Run Task` -> `Build: all targets`.

## Debug (F5)
Open Run and Debug and choose the config for your OS:

| Config | OS | Debugger |
|---|---|---|
| `Debug Selected CMake target - Linux (Build first)` | Linux / WSL | gdb |
| `Debug Selected CMake target - Linux (No Build)` | Linux / WSL | gdb |
| `Debug Selected CMake target - Windows (Build first)` | Windows | Visual Studio debugger (`cppvsdbg`) |
| `Debug Selected CMake target - Windows (No Build)` | Windows | Visual Studio debugger (`cppvsdbg`) |

- **Build first:** builds the selected target once, then starts the debugger. Use this by default.
- **No Build:** starts the existing binary without building. Use it when nothing has changed, or when you're rebuilding some other way.

All four configs show up on every machine, and picking the wrong OS's config just fails to launch. The Windows configs need the Visual Studio debugger, which comes with the C/C++ extension on Windows and needs Visual Studio or the Build Tools installed.

## Run tests
- **Test Explorer:** open the Testing panel. It uses the selected test preset and shows each test, with output on failure.
- **Debug one test:** select that test binary as the launch target and press F5.
- **Command line:**
  ```bash
  ctest --preset debug-unix_makefiles          # Linux / macOS
  ctest --preset windows-VS22                  # Windows (Debug)
  ctest --preset windows-VS22 -C Release       # Windows (Release, after building with --config Release)
  ```
New test files are picked up on the next CMake configure (`CMake: Configure`).

## Before opening a PR
Run `Tasks: Run Task` -> `Pre-PR`. It runs, in order, and stops at the first failure:
1. `Format: check` runs `scripts/format.sh --check`, the same check CI runs in its `Format Check` job. If it fails, run `Format: fix` and commit the result.
2. `Build: all targets` builds with the active build preset.
3. `Test: all` runs every test with the active test preset, including `core_independence_check`.

CI runs the same presets (`release-unix_makefiles` on Linux and macOS, `windows-VS22` and `windows-VS25` on Windows), building and testing Release from a fresh configure. Pre-PR runs the same steps, but only for your platform and the presets you selected, so select a release preset to get closest to CI. A green Pre-PR run doesn't guarantee green CI on the other platforms. The format tasks call `bash`, which on Windows needs Git Bash or WSL.

## Gotchas
- **No launch target selected:** the build task and launch configs fail or do nothing. Select a target first.
- **Don't switch the launch configs to `${command:cmake.launchTargetPath}` or `cmake.launchTargetName`.** Those variables make CMake Tools build the target while resolving them, so "No Build" would build and "Build first" would build twice. The `cmake.get...` versions are used on purpose because they only look up the name or path.
- **Compile errors:** the build tasks use both the `$gcc` and `$msCompile` problem matchers, so errors show up in the Problems panel on either platform.
