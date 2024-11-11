#pragma once

#include <string>

namespace Utility
{
    enum class LogLevel 
    {
        DEBUG = 0,
        INFO,
        WARNING,
        ERROR
    };

    // Macros to make logger calls
    #define LOG(logger, level, message) (logger).log(message, level, __FILE__, __LINE__)
    #define LOG_DEBUG(logger, message) LOG(logger, Utility::LogLevel::DEBUG, message)
    #define LOG_INFO(logger, message) LOG(logger, Utility::LogLevel::INFO, message)
    #define LOG_WARNING(logger, message) LOG(logger, Utility::LogLevel::WARNING, message)
    #define LOG_ERROR(logger, message) LOG(logger, Utility::LogLevel::ERROR, message)


    /* Log Class Interface */
    class LoggerI
    {
        public:
            virtual ~LoggerI() {}
            virtual void log(const std::string& message, Utility::LogLevel level, const char* file, int line) = 0;
    };
}