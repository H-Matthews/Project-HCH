#include "utility/inc/Logging/Logger.hpp"

// Sinks Here
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

namespace Utility
{
    // Default Constructor Initializes Sink with ColorConsole
    Logger::Logger() :
        mSinks( {std::make_shared< ColorConsoleSink >()} ),
        mLoggerName(),
        mGlobalLogLevel(LogLevel::INFO)
    {
    }

    // Constructor Sets the Logger Name
    Logger::Logger(const std::string& loggerName) :
        mSinks( {std::make_shared< ColorConsoleSink >()} ),
        mLoggerName(loggerName),
        mGlobalLogLevel(LogLevel::INFO)
    {
    }

    void Logger::log(const std::string& message, LogLevel level,
                     const char* file, int line)
    {
        bool result = false;
        for(const auto& sink : mSinks)
        {
            // Determine if we can log this
            result = shouldLog(level, sink->getSinkLogLevel());
            if(result)
            {
                // Formatter before here
                // formatter->format(message, buffer)
                sink->sinkData(message, level, file, line);
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