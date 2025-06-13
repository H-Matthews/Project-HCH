#pragma once

#include "utility/Logging/Logger.hpp"

namespace Utility
{

    /**
     * ColorConsoleSink writes output to std::cout, and inserts ANSI color codes into the buffer.
     *
     * NOTE: If the terminal does not support ANSI codes, then the output will be gibberish
     */
    class ColorConsoleSink : public LogSink
    {
        static const std::string sinkIdentifier;

      public:
        ColorConsoleSink( LogLevel level = LogLevel::NONE );

        void sinkData(
            std::string_view message, Utility::LogLevel level, const std::source_location location ) override;

        ~ColorConsoleSink()
        {}

      private:
        void insertColorCodes( std::string& message, std::string colorCode );

        const std::string getColorCode( LogLevel level ) const;

      private:
        std::ostream& mOutputStream;

        // ANSI Color Codes
        const std::string mDebugColorCode = "\033[34m";   // Blue
        const std::string mInfoColorCode = "\033[32m";    // Green
        const std::string mWarningColorCode = "\033[33m"; // Yellow
        const std::string mErrorColorCode = "\033[31m";   // Red
        const std::string mDefaultColorCode = "\033[0m";  // Reset
    };

    // Convenience function
    // Creates Logger with the necessary Sink. Registers with LogRegistry
    std::shared_ptr< Utility::Logger > createColorConsoleLogger(
        const std::string& loggerName, LogLevel level = LogLevel::NONE );
}