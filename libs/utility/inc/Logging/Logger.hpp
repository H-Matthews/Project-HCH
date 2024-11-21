#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

#include <string>
#include <vector>
#include <memory>
#include <source_location>

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
            Logger() = delete;
            Logger(const std::string& loggerName);

            void logDebug(std::string_view message, const std::source_location location = std::source_location::current());
            void logInfo(std::string_view message, const std::source_location location = std::source_location::current());
            void logWarn(std::string_view message, const std::source_location location = std::source_location::current());
            void logError(std::string_view message, const std::source_location location = std::source_location::current());

            bool shouldLog(LogLevel level, LogLevel sinkLevel);

            inline void setGlobalLogLevel(LogLevel gLevel) { mGlobalLogLevel = gLevel; }
            inline LogLevel getGlobalLogLevel() const { return mGlobalLogLevel; }

            inline void setLoggerName(const std::string& name) {mLoggerName = name; }
            inline const std::string& getLoggerName() const {return mLoggerName; }

            // Inserts a single sink into mSinks
            void addSink(std::shared_ptr< LogSinksI > sink);

            // Insert multiple sinks into mSink
            typedef std::vector< std::shared_ptr< LogSinksI > > sinkList;
            void addSinkList(sinkList list);

            std::vector< LogSinksI* > getSinkReferences();

        private:
            void sinkIt(std::string_view message, LogLevel level, const std::source_location location);

        private:
            std::vector< std::shared_ptr< LogSinksI > > mSinks;
            std::string mLoggerName;
            LogLevel mGlobalLogLevel;
    };
}