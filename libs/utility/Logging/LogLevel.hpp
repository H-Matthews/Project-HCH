#pragma once

#include <string>

namespace Utility {
enum class LogLevel { NONE = 0, DEBUG, INFO, WARN, ERROR };

const std::string logLevelEnumToString(const LogLevel identifier);
LogLevel stringToLogLevelEnum(const std::string& logLevel);
} // namespace Utility