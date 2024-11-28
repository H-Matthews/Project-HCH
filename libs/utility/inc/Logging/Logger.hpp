#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"
#include "utility/inc/Logging/LogRegistry.hpp"

#include <vector>

namespace Utility
{
    /* 
        Main Logger Class
        A sink is the logging strategy that this logger will use. You can configure more than one. See more, LogSinksI.hpp
        Global Log Level pertains to this entire Logger. You can specify a Log Level in each Sink to exclude 
        specific log levels.
        Each Logger is registered into a Logger Registry. This allows us to get the logger from any class
    */
    class Logger 
    {
        public:
            typedef std::vector< std::shared_ptr< LogSinksI > > sinkList;

            Logger() = delete;
            Logger(const std::string& loggerName, Utility::LogLevel level = Utility::LogLevel::NONE);

            Logger(const std::string& loggerName, 
                   std::shared_ptr< Utility::LogSinksI > sink,
                   Utility::LogLevel level = Utility::LogLevel::NONE);

            Logger(const std::string& loggerName, 
                   Utility::Logger::sinkList sinks,
                   Utility::LogLevel level = Utility::LogLevel::NONE);

            void logDebug(std::string_view message, const std::source_location location = std::source_location::current());
            void logInfo(std::string_view message, const std::source_location location = std::source_location::current());
            void logWarn(std::string_view message, const std::source_location location = std::source_location::current());
            void logError(std::string_view message, const std::source_location location = std::source_location::current());

            bool shouldLog(LogLevel level, LogLevel sinkLevel) const;

            inline void setGlobalLogLevel(LogLevel gLevel) { mGlobalLogLevel = gLevel; }
            inline LogLevel getGlobalLogLevel() const { return mGlobalLogLevel; }
            const std::string getGlobalLogLevelAsString() const;

            inline void setLoggerName(const std::string& name) {mLoggerName = name; }
            inline const std::string& getLoggerName() const { return mLoggerName; }

            // Inserts a single sink into mSinks
            void addSink(std::shared_ptr< LogSinksI > sink);

            // Insert multiple sinks into mSink
            void addSinkList(sinkList list);

            std::vector< LogSinksI* > getSinkReferences();

            inline bool getIsGlobalLogger() const { return mIsGlobalLogger; }

            // Define as a friend
            friend void createGlobalLogger();

        private:
            void sinkIt(std::string_view message, LogLevel level, const std::source_location location);
            void toggleGlobalLogger();

        private:
            std::vector< std::shared_ptr< LogSinksI > > mSinks;
            std::string mLoggerName;
            LogLevel mGlobalLogLevel;

            bool mIsGlobalLogger = false;
    };

    // Creates a Global Logger
    void createGlobalLogger();
}