#pragma once

#include "utility/Logging/Logger.hpp"

namespace Utility {

/**
 * ColorConsoleSink writes output to std::cout with ANSI color codes when stdout is a TTY.
 * Color codes are automatically disabled when output is redirected to a file or pipe.
 */
class ColorConsoleSink : public LogSink {
    static const std::string sinkIdentifier;

  public:
    class Builder {
      public:
        Builder();
        Builder& logLevel(const std::string& level);
        Builder& formatter(std::unique_ptr<LogFormatter> f);
        std::shared_ptr<ColorConsoleSink> build();

      private:
        std::shared_ptr<ColorConsoleSink> mSink;
    };

    static Builder make();

    ColorConsoleSink();

    void sinkData(std::string_view message, LogLevel level,
                  const std::source_location location) override;

    ~ColorConsoleSink() override = default;

  private:
    void insertColorCodes(std::string& message, const std::string& colorCode) const;

    std::string getColorCode(LogLevel level) const;

  private:
    std::ostream& mOutputStream;
    bool mUseColor;

    // ANSI Color Codes
    const std::string mDebugColorCode = "\033[34m";   // Blue
    const std::string mInfoColorCode = "\033[32m";    // Green
    const std::string mWarningColorCode = "\033[33m"; // Yellow
    const std::string mErrorColorCode = "\033[31m";   // Red
    const std::string mDefaultColorCode = "\033[0m";  // Reset
};

// Convenience function
// Creates Logger with the necessary Sink. Registers with LogRegistry
std::shared_ptr<Logger> createColorConsoleLogger(const std::string& loggerName,
                                                          LogLevel level = LogLevel::NONE);
} // namespace Utility
