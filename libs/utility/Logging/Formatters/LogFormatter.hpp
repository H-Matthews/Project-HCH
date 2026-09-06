#pragma once

#include "utility/Logging/LogLevel.hpp"

#include <source_location>

namespace Utility {
/**
 * Base class for implementing Log Message Formatters
 */
class LogFormatter {
  public:
    LogFormatter(const std::string& name) : mName(name) {}

    virtual std::string format(std::string message, LogLevel level,
                               const std::source_location location) = 0;

    virtual ~LogFormatter() {}

  private:
    std::string mName;
};
} // namespace Utility