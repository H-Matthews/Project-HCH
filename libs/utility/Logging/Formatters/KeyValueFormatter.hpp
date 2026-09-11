#pragma once

#include "utility/Logging/Formatters/LogFormatter.hpp"

namespace Utility {
enum class Keys { FILE = 0, LINE, FUNCTION, TIME, LOG_LEVEL, MESSAGE };

/**
 * KeyValueFormatter class defines a log message formatter that prints each entry as a Key:Value
 * Primarily used for TextFile based sinks
 */
class KeyValueFormatter : public LogFormatter {
  public:
    KeyValueFormatter();

    // From FormatterI
    std::string format(const std::string& message, LogLevel level,
                       const std::source_location location) override;
};

std::string keysEnumToString(const Keys identifier);
} // namespace Utility