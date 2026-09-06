#pragma once

#include <string>
#include <memory>

namespace Utility {
class ColorConsoleSink;
class LogFormatter;

class ColorConsoleSinkBuilder {
  public:
    ColorConsoleSinkBuilder(ColorConsoleSink* sink);

    ColorConsoleSinkBuilder& logLevel(const std::string& logLevel);

    ColorConsoleSinkBuilder& formatter(std::unique_ptr<LogFormatter> formatter);

    ~ColorConsoleSinkBuilder() {}

    // Conversion Function
    operator ColorConsoleSink*() { return this->mSink; }

  private:
    ColorConsoleSink* mSink;
};
} // namespace Utility