#include "utility/Logging/LogLevel.hpp"

std::string Utility::logLevelEnumToString(const LogLevel identifier) {
    std::string tempString;

    switch (identifier) {
    case LogLevel::NONE: {
        tempString = "NONE";
        break;
    }
    case LogLevel::DEBUG: {
        tempString = "DEBUG";
        break;
    }
    case LogLevel::INFO: {
        tempString = "INFO";
        break;
    }
    case LogLevel::WARN: {
        tempString = "WARN";
        break;
    }
    case LogLevel::ERROR: {
        tempString = "ERROR";
        break;
    }
    }

    return tempString;
}

Utility::LogLevel Utility::stringToLogLevelEnum(std::string_view logLevel) {
    LogLevel tempLevel = LogLevel::NONE;

    if (logLevel == "DEBUG") {
        tempLevel = LogLevel::DEBUG;
    } else if (logLevel == "INFO") {
        tempLevel = LogLevel::INFO;
    } else if (logLevel == "WARN") {
        tempLevel = LogLevel::WARN;
    } else if (logLevel == "ERROR") {
        tempLevel = LogLevel::ERROR;
    } else {
        tempLevel = LogLevel::NONE;
    }

    return tempLevel;
}