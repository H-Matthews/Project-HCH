#pragma once

#include "utility/Logging/LogLevel.hpp"

#include <source_location>

namespace Utility {
/**
 * Base class for implementing Log Message Formatters
 */
class LogFormatter {
  public:
    explicit LogFormatter(const std::string& name) : mName(name) {}

    virtual std::string format(const std::string& message, LogLevel level,
                               const std::source_location location) = 0;

    virtual ~LogFormatter() = default;

  private:
    std::string mName;
};
} // namespace Utility