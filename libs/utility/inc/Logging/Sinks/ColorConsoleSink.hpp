#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

namespace Utility 
{
    class ColorConsoleSink : public LogSinksI
    {
        public:
            ColorConsoleSink();

            void sinkData(const std::string& message, Utility::LogLevel level,
                          const char* file, int line) override;

        private:
            const std::string getColorCode(LogLevel level);

        private:
            std::ostream& mOutputStream;

            // ANSI Color Codes
            const std::string mDebugColorCode = "\033[34m";     // Blue
            const std::string mInfoColorCode = "\033[32m";      // Green
            const std::string mWarningColorCode =  "\033[33m";  // Yellow
            const std::string mErrorColorCode = "\033[31m";     // Red
            const std::string mDefaultColorCode = "\033[0m";    // Reset
    };
}