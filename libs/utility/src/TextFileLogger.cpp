#include "utility/inc/TextFileLogger.hpp"
#include "utility/inc/ConsoleLogger.hpp"
#include "utility/inc/StringOperations.hpp"

#include <iostream>
#include <filesystem>
#include <sstream>

namespace Utility
{
    TextFileLogger::TextFileLogger(const std::string& fileName, const std::string& outputDirPath) :
        mFileName(fileName),
        mOutputDirPath(outputDirPath),
        mLogExtension(".log")
    {
        auto cLogger = std::make_unique<ConsoleLogger>();
        std::stringstream logStream;

        // Append log extension to build the fileName
        mFileName += mLogExtension;

        // Build full path for file
        mFileName = mOutputDirPath + "/" + mFileName;

        mFileHandle.open(mFileName, std::ios::app);

        if(mFileHandle.is_open())
        {            
            logStream << "Creating Log File: " << mFileName;
            LOG_INFO(cLogger, logStream.str());
        }
        else
        {
            logStream << "FAILED to create file: " << mFileName;
            LOG_WARNING(cLogger, logStream.str());
        }
    }

    void TextFileLogger::log(const std::string& message, LogLevel level, const char* file, int line)
    {
        // Ensure our Handle is allocated properly
        if(mFileHandle.is_open())
        {
            // Get Time Stamp
            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            std::tm now_tm = *std::localtime(&nowTime);

            // Get LogLevel as String
            const std::string logLevelString = logLevelEnumToString(level);
    
            // Build Header
            mFileHandle << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] "
                  << "[" << logLevelString << "]";

            if(file)
            {
                // Parse the file path for JUST the file NAME
                std::filesystem::path filePath(file);
                std::string fileName = filePath.filename().string();

                mFileHandle << " [" << fileName << ":" << line << "]"; 
            }

            // Write message 
            mFileHandle << " " << message << std::endl;
        }
    }


}