#pragma once

#include "utility/Logging/Formatters/LogFormatter.hpp"

#include <filesystem>

namespace Utility {

class Logger;

/**
 * Base class for implementing a Log output strategy
 *
 * Conceptually a "Sink" defines where our log message will be output
 * Each Sink has its own formatter that defines how the message is formatted
 */
class LogSink {
  public:
    virtual void sinkData(std::string_view message, LogLevel level,
                          const std::source_location location) = 0;

    void setSinkLogLevel(LogLevel level);

    LogLevel getSinkLogLevel() const;

    void setFormatter(std::unique_ptr<LogFormatter> formatter);

    LogFormatter* getFormatter();

    virtual ~LogSink() = default;

  protected:
    explicit LogSink(const std::string& sinkIdentifier)
        : mFormatter(nullptr), mSinkIdentifier(sinkIdentifier), mLevel(LogLevel::NONE){};

    explicit LogSink(const std::string& sinkIdentifier, std::unique_ptr<LogFormatter> formatter,
                     LogLevel level = LogLevel::NONE)
        : mFormatter(std::move(formatter)), mSinkIdentifier(sinkIdentifier), mLevel(level){};

    // Pointer to the Format type
    std::unique_ptr<LogFormatter> mFormatter;

    // Each Sink has a LogLevel.
    // If its set to NONE, we use the global logger level
    const std::string mSinkIdentifier;
    LogLevel mLevel;
};

inline void LogSink::setSinkLogLevel(LogLevel level) {
    mLevel = level;
}

inline LogLevel LogSink::getSinkLogLevel() const {
    return mLevel;
}

inline void LogSink::setFormatter(std::unique_ptr<LogFormatter> formatter) {
    mFormatter.swap(formatter);
}

inline LogFormatter* LogSink::getFormatter() {
    return mFormatter.get();
}
} // namespace Utility