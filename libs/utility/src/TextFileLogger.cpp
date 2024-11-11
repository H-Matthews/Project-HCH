#include "utility/inc/TextFileLogger.hpp"

#include <iostream>

//  Define Static storage
std::map< std::size_t, std::shared_ptr<Utility::TextFileLogger> > Utility::TextFileLogger::mLoggers;

namespace Utility
{
    void TextFileLogger::establishLogger(const std::string& loggerName)
    {
        // Hash the logger name
        std::hash<std::string> hashedLogger;
        std::size_t hashedLoggerValue = hashedLogger(loggerName);

        // If NOT in map, then create object
        if(mLoggers.find(hashedLoggerValue) == mLoggers.end())
        {
            std::shared_ptr<TextFileLogger> logger = std::make_shared<TextFileLogger>();
            mLoggers.insert( {hashedLoggerValue, logger} );
        }
    }

    std::shared_ptr<TextFileLogger> TextFileLogger::getLogger(const std::string& loggerName)
    {
        std::hash<std::string> hashedLogger;
        std::size_t hashedLoggerValue = hashedLogger(loggerName);

        std::shared_ptr<TextFileLogger> logRef;

        auto it = mLoggers.find(hashedLoggerValue);
        if(it != mLoggers.end())
            logRef = mLoggers[hashedLoggerValue];

        return logRef;
    }

    void TextFileLogger::log(const std::string& message, LogLevel level, const char* file, int line)
    {
        std::cout << "This is a Test " << std::endl;
    }

}