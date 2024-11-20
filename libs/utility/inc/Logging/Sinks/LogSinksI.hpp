#pragma once

#include "utility/inc/Logging/CommonLog.hpp"

#include <string>

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
            ~LogSinksI() {}
            virtual void sinkData(const std::string& message, LogLevel level,
                                  const char* file, int line) = 0;

            inline void setSinkLogLevel(LogLevel level) { mLevel = level; }
            inline LogLevel getSinkLogLevel() { return mLevel; } 

        protected:
            // Each Sink has a LogLevel. 
            // If its set to NONE, we use the global logger level
            LogLevel mLevel = LogLevel::NONE;
    };
}