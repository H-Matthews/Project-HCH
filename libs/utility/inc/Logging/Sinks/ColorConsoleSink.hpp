#pragma once

#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

namespace Utility 
{
    /*
        This class inserts the ANSI color codes into the buffer before logging
    */
    class ColorConsoleSink : public LogSinksI
    {
        static const std::string sinkIdentifier;

        public:
            ColorConsoleSink(LogLevel level = LogLevel::NONE);

            void sinkData(std::string_view message, Utility::LogLevel level, const std::source_location location) override;

        private:
            const std::string getColorCode(LogLevel level) const;

        private:
            std::ostream& mOutputStream;

            // ANSI Color Codes
            const std::string mDebugColorCode = "\033[34m";     // Blue
            const std::string mInfoColorCode = "\033[32m";      // Green
            const std::string mWarningColorCode =  "\033[33m";  // Yellow
            const std::string mErrorColorCode = "\033[31m";     // Red
            const std::string mDefaultColorCode = "\033[0m";    // Reset
    };


    
     namespace Factory
     {
        // Convenience function
        // Creates Logger with the necessary Sink. Registers with LogRegistry
        std::shared_ptr< Utility::Logger > createColorConsoleLogger(const std::string& loggerName, LogLevel level = LogLevel::NONE);
     }
}