#include "utility/inc/Logging/Logger.hpp"

// Sinks Here
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

namespace Utility
{
    // Constructor Sets the Logger Name
    Logger::Logger(const std::string& loggerName) :
        mSinks(),
        mLoggerName(loggerName),
        mGlobalLogLevel(LogLevel::NONE)
    {
    }

    void Logger::logDebug(std::string_view message, const std::source_location location)
    {
        LogLevel level = LogLevel::DEBUG;

        // Log
        sinkIt(message, level, location);
    }

    void Logger::logInfo(std::string_view message, const std::source_location location)
    {
        LogLevel level = LogLevel::INFO;

        // Log
        sinkIt(message, level, location);
    }

    void Logger::logWarn(std::string_view message, const std::source_location location)
    {
        LogLevel level = LogLevel::WARN;

        // Log
        sinkIt(message, level, location);
    }

    void Logger::logError(std::string_view message, const std::source_location location)
    {
        LogLevel level = LogLevel::ERROR;

        // Log
        sinkIt(message, level, location);
    }

    void Logger::sinkIt(std::string_view message, LogLevel level, const std::source_location location)
    {
        bool result = false;
        for(const auto& sink : mSinks)
        {
            // Determine if we can log this
            result = shouldLog(level, sink->getSinkLogLevel());
            if(result)
            {
                sink->sinkData(message, level, location);
            }
        }
    }

    bool Logger::shouldLog(LogLevel level, LogLevel sinkLevel)
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

    void Logger::addSink(std::shared_ptr< LogSinksI > sink)
    {
        // Probably should do more checks here for potential issues that I can't think of
        if(sink != nullptr)
            mSinks.push_back(sink);
    }

    void Logger::addSinkList(sinkList list)
    {
        for(auto sink : list)
        {
            if(sink != nullptr)
                mSinks.push_back(sink);
        }
    }

    std::vector< LogSinksI* > Logger::getSinkReferences()
    {
        std::vector< LogSinksI* > test;

        for(const auto& sink : mSinks)
        {
            test.push_back(sink.get());
        }

        return test;
    }

}