#include "utility/inc/TextFileLogger.hpp"

#include <iostream>
#include <filesystem>

//  Define Static storage
std::map< std::string, std::shared_ptr<Utility::TextFileLogger> > Utility::TextFileLogger::mLoggers;

namespace Utility
{
    void TextFileLogger::establishLogger(const std::string& filePath)
    {
        // If NOT in map, then create object
        if(mLoggers.find(filePath) == mLoggers.end())
        {
            std::shared_ptr<TextFileLogger> logger = std::make_shared<TextFileLogger>();
            mLoggers.insert( {filePath, logger} );
        }
    }

    std::shared_ptr<TextFileLogger> TextFileLogger::getLogger(const std::string& loggerName)
    {
        std::shared_ptr<TextFileLogger> logRef;

        auto it = mLoggers.find(loggerName);
        if(it != mLoggers.end())
            logRef = mLoggers[loggerName];

        return logRef;
    }

    void TextFileLogger::log(const std::string& message, LogLevel level, const char* file, int line)
    {
        std::cout << "This is a Test " << std::endl;
    }

}