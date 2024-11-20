#pragma once

namespace Utility
{
    enum class LogLevel 
    {
        NONE = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    // These macros are the intended way users interface with the Logger
    #define LOG(logger, level, message) (logger)->log(message, level, __FILE__, __LINE__)
    #define LOG_DEBUG(logger, message) LOG(logger, Utility::LogLevel::DEBUG, message)
    #define LOG_INFO(logger, message) LOG(logger, Utility::LogLevel::INFO, message)
    #define LOG_WARNING(logger, message) LOG(logger, Utility::LogLevel::WARNING, message)
    #define LOG_ERROR(logger, message) LOG(logger, Utility::LogLevel::ERROR, message)

}