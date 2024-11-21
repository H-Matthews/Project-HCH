#include "utility/inc/Logging/Sinks/TextFileSink.hpp"
#include "utility/inc/StringOperations.hpp"

#include <filesystem>
#include <sstream>
#include <iostream>

namespace Utility
{
    TextFileSink::TextFileSink(const std::string& fileName, const std::string& outputDirectory, const std::string logExtension) :
        mFileName(fileName),
        mOutputDirectory(outputDirectory),
        mLogExtension(logExtension)
    {
        // Build Path
        std::stringstream filePath;
        filePath << outputDirectory << "/";
        filePath << fileName << logExtension;

        // Open File
        mFileHandle.open(filePath.str(), std::ios::app);

        // Store path 
        mEntireFilePath = filePath.str();
    }

    void TextFileSink::sinkData(std::string_view message, LogLevel level, const std::source_location location)
    {
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

            // Parse the file path for JUST the file NAME
            std::filesystem::path filePath(location.file_name());

            mFileHandle << " [" << filePath.filename().string() << ":" << location.line() << "]";

            // Write message and Flush
            mFileHandle << " " << message << std::endl;
        }
    }
}