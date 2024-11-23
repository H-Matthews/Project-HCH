#include "utility/inc/Logging/LogLevel.hpp"

const std::string Utility::logLevelEnumToString(const LogLevel identifier)
{
    std::stringstream sStream;

        switch(identifier)
        {
            case LogLevel::NONE:
            {
                sStream << "NONE";
                break;
            }
            case LogLevel::DEBUG:
            {
                sStream << "DEBUG";
                break;
            }
            case LogLevel::INFO:
            {
                sStream << "INFO";
                break;
            }
            case LogLevel::WARN:
            {
                sStream << "WARN";
                break;
            }
            case LogLevel::ERROR:
            {
                sStream << "ERROR";
                break;
            }
        }

        return sStream.str();
}