# Architecture

This document describes how Project HCH is organised: what each part of the repository is for, how the parts depend on one another, and how the program behaves at runtime. For build and run instructions see the [README](README.md).

## Contents

1. [Overview](#1-overview)
2. [Repository layout](#2-repository-layout)
3. [Layering and dependency rules](#3-layering-and-dependency-rules)
4. [Engine subsystems](#4-engine-subsystems)
5. [Game layer (HCH)](#5-game-layer-hch)
6. [Runtime flow](#6-runtime-flow)
7. [Configuration and assets](#7-configuration-and-assets)
8. [Build and test](#8-build-and-test)
9. [Extending the project](#9-extending-the-project)
10. [Further reading](#10-further-reading)

---

## 1. Overview

Project HCH is a C++20 game engine and the game built on top of it.

| Concern | Technology |
|---|---|
| Windowing, rendering, input | [SFML](https://www.sfml-dev.org/) 3.0.2 (fetched by CMake) |
| Entity Component System | [EnTT](https://github.com/skypjack/entt) (vendored, header-only) |
| Configuration files | [TOML++](https://github.com/marzer/tomlplusplus) (vendored, header-only) |
| Tests | GoogleTest (fetched by CMake) |
| Build | CMake 3.30+ with `CMakePresets.json` |

The repository produces four kinds of target:

- **`engine`**: a static library. It is game-agnostic and contains the application loop, state stack, events and input, configuration, ECS base classes and utilities.
- **`application`**: a static library holding the HCH game layer (states, scene, components, systems, key bindings). It is built on `engine`.
- **`Project-HCH`**: the game executable. Its `main.cpp` wires the engine to the game layer.
- **`Sandbox`**: a minimal executable that links only `engine`. It is for trying engine features without any game code.

The engine is the product; the game proves it. That intent drives the central design rule in [section 3](#3-layering-and-dependency-rules).

## 2. Repository layout

```
Project-HCH/
├── CMakeLists.txt            Root build: fetches SFML + GoogleTest, adds the subdirectories below
├── CMakePresets.json         Configure/build presets (Linux Makefiles, Windows VS 2022/2025)
├── .clang-format             Formatting rules (see scripts/format.sh)
├── .github/workflows/ci.yml  CI: build + test on Windows, Linux and macOS
│
├── engine/                   Target: engine (static library)
│   ├── core/
│   │   ├── Application.*         Window, main loop, lifecycle state machine
│   │   ├── StateStack/           State base class and the stack that drives it
│   │   ├── Event.hpp             Engine-owned Event variant, Key and MouseButton enums
│   │   ├── InputState.hpp        Polled key/mouse state built from events
│   │   ├── Window/               Window interface and SFML backend (+ SFML event translation)
│   │   ├── ecs/                  EntityManager, Entity, BaseSystem (wrap EnTT)
│   │   ├── Configuration/        Config loading, directory layout, logger setup
│   │   ├── GameAssetContainer/   ResourceHolder<T> (textures, fonts)
│   │   └── Exceptions/
│   ├── utility/              Logging, string operations, time, vector math
│   └── vendor/               EnTT, TOML++ (third-party, header-only)
│
├── games/HCH/                Target: application (library) + Project-HCH (executable)
│   ├── main.cpp              Composition root for the game
│   ├── configs/              root.toml, core_configurables.toml, gameplay.toml, prefabs.toml
│   ├── assets/               fonts/, textures/
│   └── src/application/      Game layer
│       ├── StateStack/           MenuState, GameState, PauseState, GameOverState
│       ├── Scene/                Scene + ECS (Components/, Systems/, EntityFactory, GameTuning)
│       ├── Data/                 Resource keys + loading, PlayerAction
│       ├── Settings/             KeyBindings
│       └── UI/                   Text helpers
│
├── sandbox/                  Target: sandbox (engine-only executable, own configs/ and assets/)
├── tests/                    GoogleTest binaries: Core/, Utility/, Application/
└── scripts/format.sh         Runs clang-format
```

**Include roots.** Engine headers are included relative to `engine/` (`"core/Application.hpp"`, `"utility/Logging/Logger.hpp"`). Game headers are included relative to `games/HCH/src` (`"application/Scene/Scene.hpp"`).

## 3. Layering and dependency rules

```mermaid
flowchart TB
    subgraph Executables
        HCH["Project-HCH<br/>games/HCH/main.cpp"]
        SBX["sandbox<br/>sandbox/main.cpp"]
    end

    APP["application (static lib)<br/>games/HCH/src/application"]

    subgraph ENG["engine (static lib)"]
        CORE["core/<br/>Application, StateStack, Event/InputState,<br/>ecs, Configuration, ResourceHolder"]
        UTIL["utility/<br/>Logging, strings, time"]
        VEND["vendor/<br/>EnTT, TOML++"]
    end

    SFML[("SFML 3.0.2")]

    HCH --> APP
    SBX --> ENG
    APP --> ENG
    CORE --> UTIL
    CORE --> VEND
    ENG --> SFML

    ENG -.->|"forbidden: never includes<br/>application/... or games/...<br/>(core_independence_check)"| APP
```

**The rule:** dependencies point from games toward the engine, never the other way. `engine/` must not `#include "application/..."` or `"games/..."`. The CTest `core_independence_check` (`tests/check_core_independence.cmake`) scans the engine and fails the build if this is violated.

Because the engine cannot name game types, it offers generic mechanisms and the game fills in the specifics:

| Concern | Engine provides | Game supplies |
|---|---|---|
| Input | `Core::Event`, `Core::Key`, `Core::InputState` | Key bindings (`Settings/KeyBindings`) that map `Core::Key` to player actions |
| Resource keys | `TextureHolder` / `FontHolder`, keyed by `std::string` | Key constants (e.g. `Fonts::ARIAL`) and file loading in `Data/GameResources.*` |
| States | `Core::State`, `Core::StateStack` (created from functor factories) | `MenuState`, `GameState`, `PauseState`, `GameOverState` |
| Root directory | `Core::ConfigSpec::rootDirectory` | The `GAME_DIR` compile definition on the game library (`SANDBOX_DIR` for the sandbox) |
| Gameplay data | Config sections and TOML reading | `gameplay.toml` and `GameTuning::fromConfig` |

## 4. Engine subsystems

### 4.1 Application

`Core::Application` is the engine's entry point. It is constructed from an `AppSpec` (window name and size, plus a `ConfigSpec`) and owns almost everything else:

```mermaid
flowchart LR
    APP["Core::Application"]

    APP --> CFG["Configuration<br/>+ DirectoryLayout"]
    APP --> INP["InputState"]
    APP --> STK["StateStack"]
    APP --> WIN["sf::RenderWindow"]
    APP --> TEX["TextureHolder"]
    APP --> FNT["FontHolder"]

    STK -. "Context (window, input, holders)" .-> APP
    LOG(["Utility::Logger / LogRegistry<br/>(cross-cutting)"])
    APP -.-> LOG
    STK -.-> LOG
```

Its lifecycle is a small state machine enforced by `requestTransition()`:

```mermaid
stateDiagram-v2
    [*] --> UNINITIALIZED
    UNINITIALIZED --> INITIALIZED : initialize()
    INITIALIZED --> RUNNING : run()
    RUNNING --> SHUTTING_DOWN
    SHUTTING_DOWN --> [*]
```

Any other transition is rejected. `pushState<T>()` is ignored while the application is still `UNINITIALIZED`.

### 4.2 StateStack

`Core::StateStack` holds a vector of `Core::State` objects. Typical use is layered screens: a pause menu on top of the game, for example.

- **Propagation is declarative.** The stack is walked top-down, and a state is reached unless a state above it blocks that channel. A state declares `blocksInput()` (states below get no events, and their `input()` reports every key up) and/or `blocksUpdates()` (states below are not updated, but still drawn). `PauseState` and `GameOverState` block both. `handleEvent` and `update` return `void`; the whole rule lives in `StateStack::handleEvent()`, `update()` and `refreshInputGating()`.
- **Input is gated per state.** `State::input()` returns an `InputView`, a thin per-state gate over the one shared `InputState`. The stack disables the view of every state beneath a `blocksInput()` state, so polled `isDown()` is blocked the same way events are.
- **Drawing** is bottom-up, so lower states stay visible beneath overlays.
- **Transitions are deferred.** `pushState<T>()`, `popState()` and `clearStates()` only enqueue a `PendingStateRequest`; the queue is applied by `applyPendingChanges()` at the end of `StateStack::update()`. This avoids invalidating the stack while it is being iterated.
- **Creation uses functor factories.** A push stores a `std::function<std::unique_ptr<State>()>` created in a templated method, so the stack itself never includes game headers.

States request changes through `State::requestStackPush<T>()`, `requestStackPop()` and `requestStateClear()`, and reach engine services through the `Context` they were constructed with: `window()`, `renderWindow()` (the SFML window, until there is a render abstraction), `input()` (`InputState`), `fonts()` and `textures()`. States are constructed as `State(StateStack&, Context)`.

### 4.3 Events and input

The engine owns its event types, so states never see SFML events. `Core::Event` is a `std::variant` of small structs (`KeyPressed`, `KeyReleased`, `TextEntered`, `MouseMoved`, `MousePressed`, `MouseReleased`, `MouseWheel`, `WindowResized`, `WindowClosed`, `FocusGained`, ...). `Core::Key` and `Core::MouseButton` are engine enums, and `Core::Overloaded` helps with `std::visit`.

```mermaid
flowchart LR
    SF["sf::Event"] --> TR["translate()<br/>SfmlEventTranslate.cpp"]
    TR --> WIN["Window::pollEvents()"]
    WIN --> APP["Application::processInput()"]
    APP --> INP["InputState::update()"]
    APP --> STK["StateStack::handleEvent()"]
```

`SfmlWindow::pollEvents()` translates SFML events into `Core::Event` and drops the ones the engine does not use. `Application::processInput()` feeds each event to `InputState` (held keys, buttons and the mouse position, reset on `FocusLost`) and then to the state stack. States react to one-shot input in `handleEvent` and poll held input with `input().isDown(Key)` during `update`.

### 4.4 Entity Component System

The generic ECS infrastructure is in `engine/core/ecs/`:

- `EntityManager` wraps an `entt::registry`, owns the registered systems and runs them (`update`) and the rendering system (`render`).
- `Entity` is a thin wrapper over `entt::entity` with `addComponent<T>()`, `getComponent<T>()` and `removeComponent<T>()`.
- `BaseSystem` is the system interface: `virtual void update(sf::Time)` and `virtual void render()`.

The engine defines no components or concrete systems. Those belong to the game ([section 5](#5-game-layer-hch)).

### 4.5 Configuration

`Core::ConfigSpec` tells the engine where to look: root directory, config directory, root config file and a `ConfigReader` implementation (`TOMLConfigReader`). `Configuration` loads the files and exposes top-level tables as `ConfigSection` objects. `DirectoryLayout` resolves the asset, config and `output/` directories relative to the game root. `LoggerBuilder` builds loggers from configuration.

Subsystems ask for their own section by name (`Application`, `StateStack`, ...) and tolerate it being absent. For rationale and details see `engine/core/Configuration/DESIGN_CONFIGURATION.md`.

### 4.6 Resources

`ResourceHolder<T>` (in `GameAssetContainer/`) stores loaded assets under string keys. `Application` exposes a `TextureHolder` and a `FontHolder`. The engine only provides the container; keys and file paths are game decisions.

### 4.7 Logging

`Utility::Logger` is created with a builder and writes to pluggable sinks (`ColorConsoleSink`, `TextFileSink`) using interchangeable formatters (`DefaultFormatter`, `KeyValueFormatter`). `LogRegistry` is a global singleton for looking up named loggers, and `Application` builds a logger per subsystem. Logging calls are guarded by compile-time constants (`Utility::CAN_LOG`, `CAN_LOG_INFO`, ...), and the minimum level is set at configure time with `-DLOG_LEVEL=<NONE|DEBUG|INFO|WARN|ERROR>` (default: `DEBUG` in Debug, `WARN` in Release). For details see `engine/utility/Logging/DESIGN_LOGGER.md`.

## 5. Game layer (HCH)

The game layer lives in `games/HCH/src/application/` and builds into the `application` library.

### 5.1 States

```mermaid
stateDiagram-v2
    [*] --> MenuState : main() pushes
    MenuState --> GameState : Enter (pop, push)
    MenuState --> [*] : Escape (stack empty, window closes)
    GameState --> PauseState : P (push, overlay)
    PauseState --> GameState : P or Escape (pop)
    PauseState --> MenuState : Backspace (clear, push)
    GameState --> GameOverState : player dies (push, overlay)
    GameOverState --> GameState : Enter, restart (clear, push)
    GameOverState --> MenuState : Escape (clear, push)
    GameState --> MenuState : Escape (pop, push)
```

`GameState` owns a `Scene` and a `PlayerInputPublisher`. When the player dies it records the final wave and score on `GameOverState` and pushes it on top.

### 5.2 Scene and systems

`Scene` owns an `EntityManager`, registers the game's systems, creates the player (via `EntityFactory`) and forwards `update` and `draw` to the manager. Shared per-run data lives in the registry context (`registry.ctx()`):

- `GameTuning`: gameplay balance values, loaded from `configs/gameplay.toml` through `GameTuning::fromConfig`.
- `GameStats`: wave, score, time until the next wave, and whether the player is dead.

Systems run in a fixed order each update, then the rendering system draws:

```mermaid
flowchart LR
    IN["PlayerInputSystem"] --> AI["EnemyAISystem"] --> MV["MovementSystem"] --> CB["CombatSystem"] --> HP["HealthSystem"] --> SP["SpawnSystem"]
    SP -. "update done" .-> RD["RenderSystem<br/>(draw)"]

    CTX[("registry.ctx()<br/>GameTuning · GameStats")]
    CTX -.- IN
    CTX -.- SP
```

Order matters: input is read first, and spawning runs last, so newly spawned entities first act on the next update.

Components (`Scene/ECS/Components/`): `Transform`, `Velocity`, `Collider`, `Health`, `Team`, `Shape`, `MeleeAttacker`, `Hitbox`, `EnemyAI` and `PlayerInputComponent`, plus `Player` and `Enemy` tag components.

### 5.3 Game input

`Settings/KeyBindings` maps `Core::Key` to `Actions::Player`. `GameState` turns input into player intent on the `Scene`: one-shot actions (attack) from `KeyPressed` events in `handleEvent`, and held actions (movement) by polling `input().isDown(key)` each `update`. `Scene::applyAction()` accumulates the intent in `PlayerInputComponent`, which `PlayerInputSystem` converts into velocity, facing and attack requests.

## 6. Runtime flow

### 6.1 Startup

```mermaid
sequenceDiagram
    participant M as main()
    participant A as Core::Application
    participant C as Configuration
    participant R as GameResources
    participant S as StateStack

    M->>M: initializeGlobalLogger()
    M->>M: build ConfigSpec (GAME_DIR, "configs", "root.toml", TOMLConfigReader)
    M->>A: construct(AppSpec)
    A->>C: load TOML files
    A->>A: DirectoryLayout, window, subsystem loggers
    M->>A: initialize()
    Note right of A: UNINITIALIZED → INITIALIZED
    M->>R: loadGameResources(app)
    Note right of R: fonts + textures into the holders
    M->>M: GameState::setTuning(GameTuning::fromConfig(...))
    M->>S: pushState<MenuState>() (deferred)
    M->>A: run()
    Note right of A: INITIALIZED → RUNNING, enter main loop
```

`ConfigurationException` and other `std::exception`s are caught in `main()`, printed to `stderr`, and the process exits with `-1`.

### 6.2 Main loop

`Application::run()` uses a fixed time step with an accumulator, so game logic is independent of frame rate. Each real-time delta is clamped to 0.25 s so that a stall (window drag, breakpoint) cannot cause a spiral of catch-up updates.

```mermaid
flowchart TD
    A(["window open?"]) -->|no| Z(["exit loop"])
    A -->|yes| B["accumulate elapsed time<br/>(clamped to 0.25 s)"]
    B --> C{"accumulated &gt; TIME_PER_FRAME?"}
    C -->|yes| D["processInput()<br/>window events to InputState + StateStack"]
    D --> F["update(TIME_PER_FRAME)<br/>StateStack::update, then apply pending push/pop/clear"]
    F --> G{"state stack empty?"}
    G -->|yes| H["close window"]
    G -->|no| C
    H --> C
    C -->|no| I["render()<br/>clear, StateStack::draw bottom-up, display"]
    I --> A
```

The application exits when the window closes or when the state stack becomes empty.

## 7. Configuration and assets

Each game (and the sandbox) has its own root directory containing `configs/`, `assets/` and, at runtime, `output/`:

| Path | Purpose |
|---|---|
| `games/HCH/configs/root.toml` | Entry file read by `Configuration` |
| `games/HCH/configs/core_configurables.toml` | Settings for engine subsystems (sections such as `Application`, `StateStack`) |
| `games/HCH/configs/gameplay.toml` | Gameplay tuning, read by `GameTuning::fromConfig` |
| `games/HCH/configs/prefabs.toml` | Reserved for entity definitions (currently a placeholder) |
| `games/HCH/assets/` | `fonts/` and `textures/` |
| `output/` | Per-run output such as log files (created at runtime, git-ignored) |

The root directory is passed in from the game (`ConfigSpec::rootDirectory`, set from `GAME_DIR`), so the engine never hard-codes a path.

## 8. Build and test

- **Targets:** `engine` → `application` → `Project-HCH`, plus `sandbox` and one executable per test file. The game library and engine compile with `-Wall -Wextra -Wpedantic` on GCC/Clang (`/W4` on MSVC).
- **Tests:** every `.cc` file under `tests/Core/`, `tests/Utility/` and `tests/Application/` is compiled into its own GoogleTest binary linked against `engine` and `application`, and registered with CTest (5 s timeout each). `core_independence_check` is registered alongside them.
- **CI:** `.github/workflows/ci.yml` uses the CMake presets to build in Release on Windows (`windows-VS22`, `windows-VS25`), Linux (GCC) and macOS (`release-unix_makefiles`), then runs all test binaries with `ctest --preset`. A separate `Format Check` job runs `scripts/format.sh --check` with clang-format 18.
- **Formatting:** use `scripts/format.sh`; `.clang-format` defines the style.

```bash
cmake --preset debug-unix_makefiles
cmake --build --preset debug-unix_makefiles
ctest --test-dir ./build --output-on-failure
```

## 9. Extending the project

| To add... | Do this |
|---|---|
| **A game state** | Derive from `Core::State` (constructor `(Core::StateStack&, Core::Context)`) in `games/HCH/src/application/StateStack/`, add the files to `GAME_LIB_SRC`/`GAME_LIB_INC` in `games/HCH/CMakeLists.txt`, and push it with `requestStackPush<T>()` from another state. |
| **A component or system** | Add the component to `Scene/ECS/Components/`; derive the system from `Core::BaseSystem` in `Scene/ECS/Systems/`; register it in `Scene::initializeGameSystems()` at the correct position in the order. |
| **An input action** | Add it to `Actions::Player`, bind a `Core::Key` in `Settings/KeyBindings`, and handle it in `Scene::applyAction()`. |
| **A tunable value** | Add it to `gameplay.toml` and read it in `GameTuning::fromConfig`. |
| **A texture or font** | Drop the file in `games/HCH/assets/`, add a key constant, and load it in `Data/GameResources.cpp`. |
| **A new game** | Create `games/<Name>/` with its own `CMakeLists.txt`, `configs/`, `assets/` and `main.cpp`; link it to `engine`; and add it to the root `CMakeLists.txt`. The sandbox is the smallest example. |
| **A test** | Add a `.cc` file to the matching `tests/` subdirectory. It is picked up automatically on the next CMake configure. |

When adding engine code, keep it free of game types. If the engine needs something game-specific, add a generic hook (a callback, an alias or a config value) and let the game supply it.

## 10. Further reading

- [README](README.md): requirements, build and run instructions
- [`engine/core/Configuration/DESIGN_CONFIGURATION.md`](engine/core/Configuration/DESIGN_CONFIGURATION.md): configuration design
- [`engine/utility/Logging/DESIGN_LOGGER.md`](engine/utility/Logging/DESIGN_LOGGER.md): logging design
- [Project wiki](https://github.com/H-Matthews/Project-HCH/wiki)
