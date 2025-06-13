#include "utility/Logging/LogLevel.hpp"

const std::string Utility::logLevelEnumToString( const LogLevel identifier )
{
    std::string tempString;

    switch ( identifier )
    {
        case LogLevel::NONE:
        {
            tempString = "NONE";
            break;
        }
        case LogLevel::DEBUG:
        {
            tempString = "DEBUG";
            break;
        }
        case LogLevel::INFO:
        {
            tempString = "INFO";
            break;
        }
        case LogLevel::WARN:
        {
            tempString = "WARN";
            break;
        }
        case LogLevel::ERROR:
        {
            tempString = "ERROR";
            break;
        }
    }

    return tempString;
}