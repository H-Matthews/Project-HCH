#pragma once

#include <sstream>
#include <string>

namespace Utility
{
    enum class LogLevel 
    {
        NONE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    const std::string logLevelEnumToString(const LogLevel identifier);
}