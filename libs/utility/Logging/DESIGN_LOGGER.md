# Logger Utility

## Responsibility

Provides structured, levelled logging with pluggable output destinations and message formatters. The subsystem has four distinct jobs, each owned by a separate component:

1. **Define** the log level vocabulary and compile-time gate constants (`LogLevel`, `Logger.hpp`)
2. **Format** log messages into a consistent string representation (`LogFormatter`, `DefaultFormatter`, `KeyValueFormatter`)
3. **Sink** formatted messages to an output destination (`LogSink`, `ColorConsoleSink`, `TextFileSink`)
4. **Route** messages through the right sinks and track named loggers globally (`Logger`, `LogRegistry`)

---

## Components

### `LogLevel`
**Files:** `LogLevel.{hpp,cpp}`

An `enum class` ordered `NONE=0, DEBUG, INFO, WARN, ERROR`. Provides two free functions:
```cpp
std::string logLevelEnumToString( LogLevel );
LogLevel    stringToLogLevelEnum( std::string_view );
```

Used throughout the subsystem for runtime level comparisons and config-driven level parsing.

---

### Compile-time gates
**File:** `Logger.hpp` (lines 10–40)

Four `constexpr bool` values derived from a single `MIN_LOG_LEVEL` constant:

```cpp
constexpr bool CAN_LOG       // true if any level is compiled in (not NONE)
constexpr bool CAN_LOG_DEBUG // true if DEBUG >= MIN_LOG_LEVEL
constexpr bool CAN_LOG_INFO  // true if INFO  >= MIN_LOG_LEVEL
constexpr bool CAN_LOG_WARN  // true if WARN  >= MIN_LOG_LEVEL
constexpr bool CAN_LOG_ERROR // true if ERROR >= MIN_LOG_LEVEL
```

`MIN_LOG_LEVEL` resolves in priority order:

| Condition | MIN_LOG_LEVEL |
|-----------|--------------|
| `-DLOG_LEVEL=<X>` passed to CMake | `X` |
| Debug build (`APP_DEBUG` defined) | `DEBUG` |
| Release build (default) | `WARN` |

Call sites wrap log statements in `if constexpr (CAN_LOG_<level>)` so both the log call **and its argument construction** are stripped by the compiler when the level is disabled. `CAN_LOG` (the meta-gate) is reserved for the 3 setup-only blocks that initialize the logger registry — it gates infrastructure, not log calls.

---

### `LogFormatter`
**File:** `Formatters/LogFormatter.hpp`

Pure interface. Receives the raw message string, log level, and `std::source_location`, and returns a formatted string:

```cpp
virtual std::string format( const std::string& message, LogLevel level,
                             const std::source_location location ) = 0;
```

Two concrete implementations:

- **`DefaultFormatter`** — human-readable single-line output: `[LEVEL] file:line function: message`
- **`KeyValueFormatter`** — structured key=value pairs suited for log aggregation: `level=WARN file=... line=... function=... message=...`

Each sink owns one formatter via `std::unique_ptr<LogFormatter>`. Swapping the formatter on a live sink is supported via `LogSink::setFormatter()`.

---

### `LogSink`
**File:** `Sinks/LogSink.hpp`

Abstract base class. Owns a `LogFormatter` and a per-sink `LogLevel`. If the sink level is `NONE`, the logger's global level governs instead (see `Logger::shouldLog`).

```cpp
virtual void sinkData( std::string_view message, LogLevel level,
                        const std::source_location location ) = 0;
```

Two concrete implementations, each with an inner `Builder` class accessed via `SinkType::make()`:

#### `ColorConsoleSink`
**Files:** `Sinks/ColorConsoleSink.{hpp,cpp}`

Writes to `std::cout`. Detects at construction whether stdout is a TTY (`isatty`/`_isatty`); ANSI color codes are applied only when writing to a terminal, and suppressed when output is piped or redirected.

Builder: `ColorConsoleSink::make().logLevel("INFO").formatter(...).build()`

#### `TextFileSink`
**Files:** `Sinks/TextFileSink.{hpp,cpp}`

Writes to a named file in the configured output directory. `Builder::build()` calls `openFile()` automatically so the file is ready immediately after construction.

Flush policy:
- **Debug builds** (`APP_DEBUG` defined): flushes every line (`std::endl`) for live log visibility.
- **Release builds**: buffered writes (`'\n'`) with an explicit `flush()` on `WARN` and above, so critical messages are durable while routine output stays performant.

Builder: `TextFileSink::make().outputDirectory(...).fileName(...).logExtension(".log").logLevel("INFO").formatter(...).build()`

---

### `Logger`
**Files:** `Logger.{hpp,cpp}`

The central log-routing class. Holds a list of `shared_ptr<LogSink>` and a global log level. A log call reaches a sink only when `shouldLog` passes for that sink:

```
shouldLog(messageLevel, sinkLevel):
  true  if sinkLevel != NONE && messageLevel >= sinkLevel
  also  true if globalLogLevel != NONE && messageLevel >= globalLogLevel
```

Each log method has a compile-time short-circuit inside the method body as a defensive no-op for any call site that bypasses the `if constexpr` wrapper:

```cpp
void logDebug( std::string_view message, ... ) { if constexpr (CAN_LOG_DEBUG) sinkIt(...); }
void logInfo ( std::string_view message, ... ) { if constexpr (CAN_LOG_INFO)  sinkIt(...); }
void logWarn ( std::string_view message, ... ) { if constexpr (CAN_LOG_WARN)  sinkIt(...); }
void logError( std::string_view message, ... ) { if constexpr (CAN_LOG_ERROR) sinkIt(...); }
```

Built via inner `Builder` class:
```cpp
auto logger = Utility::Logger::make()
                  .name( "MyLogger" )
                  .globalLogLevel( LogLevel::WARN )
                  .sinks( sinkList )
                  .build();                          // returns shared_ptr<Logger>
```

**Global logger.** `createGlobalLogger()` constructs a special no-sink logger named `"cLogger"` and marks it via `toggleGlobalLogger()`. `LogRegistry::getGlobalLogger()` retrieves it by scanning for the flag. It is used internally by `LogRegistry` to warn on name collisions. The global logger has no sinks by default; the application is expected to attach a `ColorConsoleSink` to it in `main` for console output.

---

### `LogRegistry`
**Files:** `LogRegistry.{hpp,cpp}`

A singleton (`instance()` pattern) that maps hashed logger names to `shared_ptr<Logger>`. Subsystems retrieve their logger by name after construction.

Key methods:
```cpp
static shared_ptr<LogRegistry> instance();
void configureRegistry( const std::string& outputDirPath ); // called before any TextFileSink is built
void registerLogger( shared_ptr<Logger> logger );           // called by buildLogger / factory helpers
shared_ptr<Logger> getLogger( const std::string& name );    // returns nullptr if not found
shared_ptr<Logger> getGlobalLogger() const;                 // returns the flagged global logger
const std::string  getOutputDir() const;
```

`configureRegistry` must be called before any `TextFileSink` is constructed; `TextFileSink::Builder` reads the output directory from the registry when building.

---

## Convenience factory functions

Two free functions in the sink headers build a complete logger (sink + logger + registration) in one call:

```cpp
// Utility/Sinks/ColorConsoleSink.hpp
shared_ptr<Logger> createColorConsoleLogger( const string& loggerName, LogLevel level = NONE );

// Utility/Sinks/TextFileSink.hpp
shared_ptr<Logger> createTextFileLogger( const string& loggerName,
    const string& outputDirectory, const string& fileName,
    const string& logExtension, LogLevel level = NONE );
```

Used by `main.cpp` and tests for lightweight logger setup without the full Builder chain.

---

## Typical setup flow

```
main.cpp
  │  createGlobalLogger()           ← constructs cLogger (no sinks yet)
  │  getGlobalLogger()->addSink(ColorConsoleSink)  ← enables console output
  ▼
Application ctor — member initializer list
  ├── MessageNetwork ctor           ← stores ConfigSection*, mLogger = nullptr
  └── StateStack ctor               ← stores ConfigSection*, mLogger = nullptr
  ▼
Application ctor — body
  │  LogRegistry::configureRegistry(outputDir)  ← sets output path for file sinks
  │  buildLogger(*appSection)       ← constructs Application's named logger
  │  buildSubsystemLoggers()
  │    ├── MessageNetwork::initializeLogger()  ← constructs MessageNetwork's named logger
  │    └── StateStack::initializeLogger()      ← constructs StateStack's named logger
  ▼
Engine runs — each subsystem holds a shared_ptr<Logger>; LogRegistry holds shared ownership
```

Subsystem loggers are intentionally deferred out of their constructors so that `configureRegistry` is guaranteed to have run before any `TextFileSink` is built. The `initializeLogger()` methods are no-ops if `mConfig` is null or if logging is compiled out (`CAN_LOG = false`).

---

## Key Design Decisions

**Compile-time vs runtime filtering.** `MIN_LOG_LEVEL` / `CAN_LOG_*` strip code at compile time — the binary never contains the call or the string arguments for disabled levels. `shouldLog()` is a separate, orthogonal runtime filter driven by per-sink and per-logger levels set from config. Both layers are active independently.

**Builder pattern on sinks and logger.** Sinks and `Logger` expose a nested `Builder` via a static `make()` factory. `Builder::build()` returns a `shared_ptr<>`, never a raw pointer. This avoids partially-constructed objects and makes ownership explicit at the construction site.

**Per-sink formatters.** Each sink owns its formatter exclusively. A `ColorConsoleSink` defaults to `DefaultFormatter` for human readability; a `TextFileSink` defaults to `KeyValueFormatter` for machine parseability. The formatter can be swapped on a live sink.

**Registry ownership.** `LogRegistry` holds `shared_ptr<Logger>` for all registered loggers. Subsystems also hold a `shared_ptr<Logger>` member. Loggers are destroyed when both the subsystem and the registry release them — typically at shutdown.

**No macro-based logging.** All log calls are ordinary member function calls. The compile-time no-op path is achieved via `if constexpr` inside the method body, meaning the call overhead is eliminated by the compiler but argument evaluation at the call site is also suppressed only when the call site is itself wrapped in a `CAN_LOG_*` guard. This is the explicit trade-off for avoiding macros.
