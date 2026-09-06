#pragma once

#include "utility/Logging/Sinks/LogSink.hpp"
#include "utility/Logging/Builder/LoggerBuilder.hpp"

#include <vector>

namespace Utility {

// APP_DEBUG is defined by the CMake build
#ifdef APP_DEBUG
constexpr bool CAN_LOG(true);
#else
constexpr bool CAN_LOG(false);
#endif

/**
 * Main Logger Class
 *
 * Loggers require Sinks to be attached in order to log a message
 * A logger with no defined sinks will not log messages
 */
class Logger {
  public:
    typedef std::vector<std::shared_ptr<LogSink>> SinkList;

    Logger();
    Logger(const std::string& loggerName, Utility::LogLevel level = Utility::LogLevel::NONE);

    Logger(const std::string& loggerName, std::shared_ptr<Utility::LogSink> sink,
           Utility::LogLevel level = Utility::LogLevel::NONE);

    Logger(const std::string& loggerName, Utility::Logger::SinkList sinks,
           Utility::LogLevel level = Utility::LogLevel::NONE);

    void logDebug(std::string_view message,
                  const std::source_location location = std::source_location::current());
    void logInfo(std::string_view message,
                 const std::source_location location = std::source_location::current());
    void logWarn(std::string_view message,
                 const std::source_location location = std::source_location::current());
    void logError(std::string_view message,
                  const std::source_location location = std::source_location::current());

    bool shouldLog(LogLevel level, LogLevel sinkLevel) const;

    inline void setGlobalLogLevel(LogLevel gLevel);

    inline LogLevel getGlobalLogLevel() const;
    inline const std::string getGlobalLogLevelAsString() const;

    inline void setLoggerName(const std::string& name);
    inline const std::string& getLoggerName() const;

    // Inserts a single sink into mSinks
    void addSink(std::shared_ptr<LogSink> sink);

    // Insert multiple sinks into mSink
    void addSinkList(SinkList list);

    std::vector<LogSink*> getSinkReferences();

    inline bool getIsGlobalLogger() const { return mIsGlobalLogger; }

    // Define as a friend
    friend void createGlobalLogger();

    inline static LoggerBuilder build();
    friend class LoggerBuilder;

  private:
    void sinkIt(std::string_view message, LogLevel level, const std::source_location location);
    void toggleGlobalLogger();

  private:
    SinkList mSinks;
    std::string mLoggerName;
    LogLevel mGlobalLogLevel;

    bool mIsGlobalLogger = false;
};

void Logger::setGlobalLogLevel(Utility::LogLevel gLevel) {
    mGlobalLogLevel = gLevel;
}

LogLevel Logger::getGlobalLogLevel() const {
    return mGlobalLogLevel;
}

const std::string Logger::getGlobalLogLevelAsString() const {
    const std::string loggerAsString = Utility::logLevelEnumToString(mGlobalLogLevel);
    return loggerAsString;
}

void Logger::setLoggerName(const std::string& name) {
    mLoggerName = name;
}

const std::string& Logger::getLoggerName() const {
    return mLoggerName;
}

Utility::LoggerBuilder Utility::Logger::build() {
    Logger* logger = new Logger();
    return Utility::LoggerBuilder(logger);
}

// Creates a Global Logger
void createGlobalLogger();
} // namespace Utility