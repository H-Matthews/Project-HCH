#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

#include <string>
#include <vector>
#include <memory>

namespace Utility
{
    /* 
        Main Logger Class
        A sink is the logging strategy that this logger will use. You can configure more than one. See more, LogSinksI.hpp
        Global Log Level pertains to this entire Logger. You can specify a Log Level in each Sink to exclude 
        specific log levels
    */
    class Logger 
    {
        public:
            Logger();
            Logger(const std::string& loggerName);
            void log(const std::string& message, LogLevel level,
                     const char* file, int line);

            bool shouldLog(LogLevel level, LogLevel sinkLevel);

            inline void setGlobalLogLevel(LogLevel gLevel) { mGlobalLogLevel = gLevel; }
            inline LogLevel getGlobalLogLevel() const { return mGlobalLogLevel; }

            inline void setLoggerName(const std::string& name) {mLoggerName = name; }
            inline const std::string& getLoggerName() const {return mLoggerName; }

            std::vector< LogSinksI* > getSinkReferences();

        private:
            std::vector< std::shared_ptr< LogSinksI > > mSinks;
            std::string mLoggerName;
            LogLevel mGlobalLogLevel;
    };
}