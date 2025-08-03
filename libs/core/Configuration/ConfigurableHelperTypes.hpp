#pragma once

#include <string>
#include <map>

namespace Core
{
    struct LoggerSinkConfigurable
    {
        std::string logLevel;
        std::string logFileName;
        std::string logFileExtension;
    };

    struct LoggerConfigurable
    {
        std::string loggerName;
        std::string globalLogLevel;
        std::map< std::string, LoggerSinkConfigurable > sinks;
    };
}