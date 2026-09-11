#pragma once

#include "utility/Logging/Sinks/LogSink.hpp"

#include <vector>

namespace Utility {

// This define can be passed as a CMake Define to control the LOG_LEVEL gates
// If building in DEBUG and no LOG_LEVEL is defined via CMake, then it defaults to DEBUG;
// Release defaults to WARN.
//
// If a LOG_LEVEL is defined via CMake, then its set to that level
#if defined(LOG_LEVEL_NONE)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::NONE);
#elif defined(LOG_LEVEL_DEBUG)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::DEBUG);
#elif defined(LOG_LEVEL_INFO)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::INFO);
#elif defined(LOG_LEVEL_WARN)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::WARN);
#elif defined(LOG_LEVEL_ERROR)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::ERROR);
#elif defined(APP_DEBUG)
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::DEBUG);
#else
constexpr LogLevel MIN_LOG_LEVEL(LogLevel::WARN);
#endif

// True if MIN_LOG_LEVEL is NOT None
constexpr bool CAN_LOG(MIN_LOG_LEVEL != LogLevel::NONE);

// Per-level gates: Wraps call sites so message construction is also stripped
// when the level is disabled.
constexpr bool CAN_LOG_DEBUG(CAN_LOG&& LogLevel::DEBUG >= MIN_LOG_LEVEL);
constexpr bool CAN_LOG_INFO(CAN_LOG&& LogLevel::INFO >= MIN_LOG_LEVEL);
constexpr bool CAN_LOG_WARN(CAN_LOG&& LogLevel::WARN >= MIN_LOG_LEVEL);
constexpr bool CAN_LOG_ERROR(CAN_LOG&& LogLevel::ERROR >= MIN_LOG_LEVEL);

/**
 * Main Logger Class
 *
 * Loggers require Sinks to be attached in order to log a message
 * A logger with no defined sinks will not log messages
 */
class Logger {
  public:
    using SinkList = std::vector<std::shared_ptr<LogSink>>;

    class Builder {
      public:
        Builder();
        Builder& name(std::string n);
        Builder& globalLogLevel(LogLevel level);
        Builder& sinks(SinkList sinks);
        std::shared_ptr<Logger> build();

      private:
        std::shared_ptr<Logger> mLogger;
    };

    static Builder make();

    Logger();

    // Creates a logger with no sinks
    explicit Logger(std::string loggerName, LogLevel level = LogLevel::NONE);

    // Creates a logger with a single sink
    Logger(std::string loggerName, std::shared_ptr<LogSink> sink,
           LogLevel level = LogLevel::NONE);

    // Creates a logger with potentiall multiple sinks
    Logger(std::string loggerName, SinkList sinks,
           LogLevel level = LogLevel::NONE);

    void logDebug(std::string_view message,
                  const std::source_location location = std::source_location::current());
    void logInfo(std::string_view message,
                 const std::source_location location = std::source_location::current());
    void logWarn(std::string_view message,
                 const std::source_location location = std::source_location::current());
    void logError(std::string_view message,
                  const std::source_location location = std::source_location::current());

    bool shouldLog(LogLevel level, LogLevel sinkLevel) const;

    void setGlobalLogLevel(LogLevel gLevel);

    LogLevel getGlobalLogLevel() const;
    std::string getGlobalLogLevelAsString() const;

    void setLoggerName(std::string name);
    const std::string& getLoggerName() const;

    // Inserts a single sink into mSinks
    void addSink(std::shared_ptr<LogSink> sink);

    // Insert multiple sinks into mSink
    void addSinkList(SinkList list);

    std::vector<LogSink*> getSinkReferences();

    inline bool getIsGlobalLogger() const { return mIsGlobalLogger; }

    // Define as a friend
    friend void createGlobalLogger();

  private:
    void sinkIt(std::string_view message, LogLevel level, const std::source_location location);
    void toggleGlobalLogger();

  private:
    SinkList mSinks;
    std::string mLoggerName;
    LogLevel mGlobalLogLevel;

    bool mIsGlobalLogger = false;
};

inline void Logger::setGlobalLogLevel(LogLevel gLevel) {
    mGlobalLogLevel = gLevel;
}

inline LogLevel Logger::getGlobalLogLevel() const {
    return mGlobalLogLevel;
}

inline std::string Logger::getGlobalLogLevelAsString() const {
    const std::string loggerAsString = logLevelEnumToString(mGlobalLogLevel);
    return loggerAsString;
}

inline void Logger::setLoggerName(std::string name) {
    mLoggerName = std::move(name);
}

inline const std::string& Logger::getLoggerName() const {
    return mLoggerName;
}

// Creates a Global Logger
void createGlobalLogger();
} // namespace Utility
