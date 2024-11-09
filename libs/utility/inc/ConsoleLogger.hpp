#pragma once

#include "utility/inc/LoggerI.hpp"

namespace Utility 
{
    class ConsoleLogger : public LoggerI
    {
        public:
            // Defaults the stream to std::cout 
            ConsoleLogger();

            // Lets the User specify the output stream
            // Specifically for Unit Testing, but it can have other uses
            ConsoleLogger(std::ostream& oStream);

            void log(const std::string& message, LogLevel level, const char* file, int line) override;

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