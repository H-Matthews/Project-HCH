#pragma once

#include "utility/inc/Logging/Formatters/LogFormatter.hpp"

#include <filesystem>

namespace Utility
{

    class Logger;

    /**
     * Base class for implementing a Log output strategy
     * 
     * Conceptually a "Sink" defines where our log message will be output
     * Each Sink has its own formatter that defines how the message is formatted
     */
    class LogSink
    {
        public:
            virtual void sinkData(std::string_view message, LogLevel level, const std::source_location location) = 0;

            // Set the formatter type
            void addFormatter(std::unique_ptr< LogFormatter > formatter);

            inline void setSinkLogLevel(LogLevel level) { mLevel = level; }
            inline LogLevel getSinkLogLevel() { return mLevel; }

            inline void setFormatter(std::unique_ptr<LogFormatter> formatter) { mFormatter.swap(formatter); }
            inline LogFormatter* getFormatter() { return mFormatter.get(); }

            virtual ~LogSink() {}

        protected:
            inline LogSink(const std::string sinkIdentifier, std::unique_ptr<LogFormatter> formatter, LogLevel level = LogLevel::NONE) : 
                mFormatter(std::move(formatter)),
                mSinkIdentifier(sinkIdentifier),
                mLevel(level) {};

            // Pointer to the Format type
            std::unique_ptr<LogFormatter> mFormatter;

            // Each Sink has a LogLevel. 
            // If its set to NONE, we use the global logger level
            const std::string mSinkIdentifier;
            LogLevel mLevel;
    };
}