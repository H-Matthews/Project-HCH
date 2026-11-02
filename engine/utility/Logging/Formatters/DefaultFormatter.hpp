#pragma once

#include "utility/Logging/Formatters/LogFormatter.hpp"

namespace Utility {
/**
 * DefaultFormatter class defines a log message formatter that prints each entry around []
 * Primarily used for logging to consoles
 */
class DefaultFormatter : public LogFormatter {
  public:
    static constexpr std::string_view IDENTIFIER = "DefaultFormatter";

    DefaultFormatter();

    // From FormatterI
    std::string format(const std::string& message, LogLevel level,
                       const std::source_location location) override;
};
} // namespace Utility