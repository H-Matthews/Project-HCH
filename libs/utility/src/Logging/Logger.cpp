#include "utility/inc/Logging/Logger.hpp"

// Creates a logger with no sinks
Utility::Logger::Logger(const std::string& loggerName, LogLevel level) :
    mSinks(),
    mLoggerName(loggerName),
    mGlobalLogLevel(level) 
{
}

// Creates a logger with a single sink
Utility::Logger::Logger(const std::string& loggerName, 
                        std::shared_ptr< LogSinksI > sink,
                        LogLevel level) :
    mSinks({ sink }),
    mLoggerName(loggerName),
    mGlobalLogLevel(level)
{
}

// Creates a logger with a sinkList
Utility::Logger::Logger(const std::string& loggerName, 
                        Logger::sinkList sinks,
                        LogLevel level) :
    mSinks(sinks),
    mLoggerName(loggerName),
    mGlobalLogLevel(level)
{
}

void Utility::Logger::logDebug(std::string_view message, const std::source_location location)
{
    LogLevel level = LogLevel::DEBUG;

    // Log
    sinkIt(message, level, location);
}

void Utility::Logger::logInfo(std::string_view message, const std::source_location location)
{
    LogLevel level = LogLevel::INFO;

    // Log
    sinkIt(message, level, location);
}

void Utility::Logger::logWarn(std::string_view message, const std::source_location location)
{
    LogLevel level = LogLevel::WARN;

    // Log
    sinkIt(message, level, location);
}

void Utility::Logger::logError(std::string_view message, const std::source_location location)
{
    LogLevel level = LogLevel::ERROR;

    // Log
    sinkIt(message, level, location);
}

void Utility::Logger::sinkIt(std::string_view message, LogLevel level, const std::source_location location)
{
    bool result = false;
    for(const auto& sink : mSinks)
    {
        // Determine if we can log this
        result = shouldLog(level, sink->getSinkLogLevel());
        if(result)
            sink->sinkData(message, level, location);
    }
}

void Utility::Logger::toggleGlobalLogger()
{
    mIsGlobalLogger = true;
}

bool Utility::Logger::shouldLog(LogLevel level, LogLevel sinkLevel) const
{
    bool canLog = false;

    // First, check to see if sink level is high enough
    if(sinkLevel != LogLevel::NONE)
    {
        if(level >= sinkLevel)
            canLog = true;
    }

    // If still false, check to see if global allows us to log
    if(!canLog && mGlobalLogLevel != LogLevel::NONE)
    {
        if(level >= mGlobalLogLevel)
            canLog = true;
    }

    return canLog;
}

const std::string Utility::Logger::getGlobalLogLevelAsString() const
{
    const std::string loggerAsString = Utility::logLevelEnumToString(mGlobalLogLevel);

    return loggerAsString;
}

void Utility::Logger::addSink(std::shared_ptr< LogSinksI > sink)
{
    // Probably should do more checks here for potential issues that I can't think of
    if(sink != nullptr)
        mSinks.push_back(sink);
}

void Utility::Logger::addSinkList(sinkList list)
{
    for(auto sink : list)
    {
        if(sink != nullptr)
            mSinks.push_back(sink);
    }
}

std::vector< Utility::LogSinksI* > Utility::Logger::getSinkReferences()
{
    std::vector< LogSinksI* > test;

    for(const auto& sink : mSinks)
    {
        test.push_back(sink.get());
    }

    return test;
}

bool Utility::Logger::getIsGlobalLogger() const
{
    return mIsGlobalLogger;
}

// NOT Apart of the class, but a friend
void Utility::createGlobalLogger()
{
    static bool initialized = false;
    if(!initialized)
    {
        initialized = true;

        auto globalConsoleLogger = std::make_shared< Utility::Logger >("cLogger", Utility::LogLevel::DEBUG);
        globalConsoleLogger->toggleGlobalLogger();

        if(globalConsoleLogger)
            Utility::LogRegistry::instance()->registerLogger(globalConsoleLogger);
    }
}