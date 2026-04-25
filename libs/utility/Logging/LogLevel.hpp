#pragma once

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

    std::string logLevelEnumToString( const LogLevel identifier );
    LogLevel stringToLogLevelEnum( std::string_view logLevel );
}