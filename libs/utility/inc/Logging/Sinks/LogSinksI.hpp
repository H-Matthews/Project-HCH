#pragma once

#include "utility/inc/Logging/LogLevel.hpp"

#include <string>
#include <source_location>
#include <memory>

namespace Utility
{
    /* 
        Defines the Sink Interface. Conceptually a sink is the final destination of the log message.
        The terminology comes from the spdlog library.
        Ex. A TextFile sink would take the message and put it in a textfile.
        Ex. A Console Logger sink would take the message and write it to the console.
    */
    class LogSinksI 
    {
        public:
            ~LogSinksI() = default;
            virtual void sinkData(std::string_view message, LogLevel level, const std::source_location location) = 0;

            inline void setSinkLogLevel(LogLevel level) { mLevel = level; }
            inline LogLevel getSinkLogLevel() { return mLevel; } 

        protected:
            // Each Sink has a LogLevel. 
            // If its set to NONE, we use the global logger level
            LogLevel mLevel = LogLevel::NONE;
    };
}