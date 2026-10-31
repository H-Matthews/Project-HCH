# Configuration Subsystem

## Responsibility

Provides the rest of the engine with typed, format-agnostic access to values declared in config files. The subsystem has three distinct jobs, each owned by a separate component:

1. **Parse** config files into an in-memory representation (`Configuration`)
2. **Serve** values to subsystems through a stable, format-agnostic interface (`ConfigSection`)
3. **Act** on specific config values that affect the environment (`DirectoryLayout`, `LoggerBuilder`)

---

## Components

### `ConfigReader` / `TOMLConfigReader`
**Files:** `ConfigReader/ConfigReader.hpp`, `ConfigReader/TOMLConfigReader.{hpp,cpp}`

`ConfigReader` is a pure interface with a single method:
```cpp
std::unique_ptr<ConfigSection> readFile(const std::filesystem::path&);
```

`TOMLConfigReader` is the only concrete implementation. It parses a `.toml` file using toml++ and returns the root table wrapped in a `TOMLConfigSection`.

The caller (via `ConfigSpec`) decides which `ConfigReader` to inject, so swapping file formats requires no changes to `Configuration` or any subsystem. Its just a matter of implementing a `ConfigReader`, and the `ConfigSection` interfaces to support the new file format.

---

### `ConfigSection` / `TOMLConfigSection`
**Files:** `ConfigSection/ConfigSection.hpp`, `ConfigSection/TOMLConfigSection.{hpp,cpp}`

`ConfigSection` is a pure interface representing one table of parsed configuration. It exposes typed value accessors and nested section navigation:

```cpp
std::optional<bool>        getBool(string_view key)
std::optional<int>         getInt(string_view key)
std::optional<double>      getDouble(string_view key)
std::optional<std::string> getString(string_view key)
vector<string>             getStringVector(string_view key)
unique_ptr<ConfigSection>  getSection(string_view name)
vector<string>             sectionNames()
```

`TOMLConfigSection` wraps a `toml::table` and delegates all calls to toml++. It is the only concrete implementation and the only file outside `TOMLConfigReader` that includes toml++ headers.

Subsystems depend only on `ConfigSection`. They never include toml++ and are unaware of the underlying file format.

---

### `Configuration`
**Files:** `Configuration.{hpp,cpp}`

Owns the full parse lifecycle. Constructed once by `Application` via a `ConfigSpec`.

**What it does:**
- Validates the config directory and reader before parsing
- Parses the root file to discover additional config files (declared as a `files` array under `[Configuration_Files]`)
- Parses all additional config files
- Builds a flat name-to-section index from all parsed file roots
- Detects and rejects duplicate top-level section names across files

**What it does not do:**
- Create directories or touch the filesystem beyond reading config files
- Bootstrap loggers or interact with `LogRegistry`

**`ConfigSpec`** is the move-only init struct passed to `Configuration`'s constructor. It carries the config directory path, root filename, and the `ConfigReader` implementation to use.

**`getSection(string_view) → const ConfigSection*`** is a non-allocating map lookup. All sections are owned internally and pre-indexed at parse time. The returned pointer is valid for the lifetime of the `Configuration` object and is non-owning — callers must not delete it.

```
root.toml
├── [Configuration]          ← read by DirectoryLayout
├── [Configuration_Files]    ← consumed internally by Configuration
│   └── files = [...]        ← list of additional config files to parse
│
core_configurables.toml
├── [Application]            ← served to Application
├── [StateStack]             ← served to StateStack
├── [MessageNetwork]         ← served to MessageNetwork
└── [Scene]                  ← served to Scene
```

---

### `DirectoryLayout`
**Files:** `DirectoryLayout.{hpp,cpp}`

Resolves and creates the on-disk directories the engine depends on (output, assets, fonts, textures). Constructed once by `Application` immediately after `Configuration`, and passed the `[Configuration]` section.

Directory paths may be overridden via config keys; if absent, hardcoded defaults are used. Construction either fully succeeds or throws `ConfigurationException` — there is no partially-initialised state.

Exposes read-only accessors for each resolved path:
```cpp
outputDirectory()        // e.g. output/App_2026-01-01_12-00-00/
assetDirectory()
assetFontsDirectory()
assetTexturesDirectory()
```

`Application` calls `LogRegistry::configureRegistry(mDirectories.outputDirectory())` after construction so file-backed log sinks can be created.

---

### `LoggerBuilder`
**Files:** `LoggerBuilder.{hpp,cpp}`

A free function `Core::buildLogger(const ConfigSection&)` that reads a subsystem's logger configuration and constructs a `Utility::Logger` with its sinks and formatters. Returns `nullptr` if logging is disabled or the `[Logger]` sub-section is absent.

Each subsystem that wants a logger calls this once in its constructor with its own `ConfigSection`. The resulting `Logger` is stored as a `shared_ptr<Utility::Logger>` member.

Expected config shape per subsystem:
```toml
[SubsystemName]
logging_enabled = true

[SubsystemName.Logger]
logger_name      = "SubsystemLogger"
global_log_level = "NONE"
sinks            = ["ColorConsoleSink", "TextFileSink"]

[SubsystemName.Logger.ColorConsoleSink]
log_level  = "INFO"
formatter  = "DefaultFormatter"

[SubsystemName.Logger.TextFileSink]
log_file_name      = "Subsystem"
log_file_extension = ".log"
log_level          = "INFO"
formatter          = "KeyValueFormatter"
```

---

## Data Flow

```
main.cpp
  │  creates ConfigSpec (picks TOMLConfigReader)
  ▼
Application ctor
  │  constructs Configuration  ──► parses files, builds section index
  │  constructs DirectoryLayout ──► creates output/asset dirs on disk
  │  calls LogRegistry::configureRegistry(outputDir)
  │  constructs MessageNetwork(config*)  ──► calls buildLogger
  │  constructs StateStack(app, config*) ──► calls buildLogger
  │  calls buildLogger for Application itself
  ▼
Engine runs — subsystems hold Logger; Configuration holds section data
```

---

## Key Design Decisions

**Format isolation.** Only `TOMLConfigReader.cpp` and `TOMLConfigSection.cpp` include toml++ headers. Everything else depends on the `ConfigReader` and `ConfigSection` interfaces. Swapping to JSON or YAML requires only a new pair of implementation files.

**Non-owning section pointers.** `Configuration::getSection` returns `const ConfigSection*`. The sections are owned by `Configuration` and live for its lifetime, which encompasses all subsystems. This avoids per-call allocation and makes the ownership relationship explicit.

**Fail-fast construction.** `Configuration`, `DirectoryLayout`, and `LoggerBuilder` all throw `ConfigurationException` on any unrecoverable error. There are no error codes or partial states — construction either succeeds completely or the exception propagates to `main`.

**Duplicate section detection.** If two config files declare the same top-level section name (e.g. `[Application]` in both `core_configurables.toml` and a second file), `Configuration` throws immediately with a message identifying both files. Silent shadowing is not possible.
