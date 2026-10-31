#pragma once

#include "utility/Logging/Logger.hpp"

#include <fstream>

namespace Utility {
/**
 * TextFileSink writes output to a specific file
 */
class TextFileSink : public LogSink {
    static const std::string SINK_IDENTIFIER;

  public:
    class Builder {
      public:
        Builder();
        Builder& outputDirectory(std::string outputDirectory);
        Builder& fileName(std::string fileName);
        Builder& logExtension(std::string logExtension);
        Builder& logLevel(const std::string& level);
        Builder& formatter(std::unique_ptr<LogFormatter> f);
        std::shared_ptr<TextFileSink> build();

      private:
        std::shared_ptr<TextFileSink> mSink;
    };

    static Builder make();

    TextFileSink();
    TextFileSink(const std::string& outputDirectory, const std::string& fileName,
                 const std::string& logExtension, LogLevel level = LogLevel::NONE);

    std::string getFilePath() const;

    void sinkData(std::string_view message, LogLevel level,
                  const std::source_location location) override;

    void openFile();

    ~TextFileSink() override = default;

  private:
    std::string mOutputDirectory;
    std::string mFileName;
    std::string mLogExtension;
    std::string mEntireFilePath;

    std::ofstream mFileHandle;
};

// Convenience function
// Creates Logger with the necessary Sink. Registers with LogRegistry
std::shared_ptr<Logger> createTextFileLogger(const std::string& loggerName,
                                             const std::string& outputDirectory,
                                             const std::string& fileName,
                                             const std::string& logExtension,
                                             LogLevel level = LogLevel::NONE);
} // namespace Utility
