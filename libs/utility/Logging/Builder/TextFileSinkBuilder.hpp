#pragma once

#include <string>
#include <memory>

namespace Utility {
class TextFileSink;
class LogFormatter;

class TextFileSinkBuilder {
  public:
    TextFileSinkBuilder(TextFileSink* sink);

    TextFileSinkBuilder& outputDirectory(const std::string& outputDirectory);

    TextFileSinkBuilder& fileName(const std::string& fileName);

    TextFileSinkBuilder& logExtension(const std::string& logExtension);

    TextFileSinkBuilder& logLevel(const std::string& logLevel);

    TextFileSinkBuilder& formatter(std::unique_ptr<LogFormatter> formatter);

    ~TextFileSinkBuilder() {}

    // Conversion Function
    operator TextFileSink*() { return this->mSink; }

  private:
    TextFileSink* mSink;
};
} // namespace Utility