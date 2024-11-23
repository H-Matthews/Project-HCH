#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <iostream>

namespace Utility
{
    TextFileSink::TextFileSink(const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension) :
        mOutputDirectory(outputDirectory),
        mFileName(fileName),
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

    const std::string TextFileSink::getFilePath() const
    {
        return mEntireFilePath;
    }

    void TextFileSink::sinkData(std::string_view message, LogLevel level, const std::source_location location)
    {
        if(mFileHandle.is_open())
        {
            // Get Time Stamp
            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            std::tm now_tm = *std::localtime(&nowTime);

            const std::string logLevelString = logLevelEnumToString(level);
            std::filesystem::path filePath(location.file_name());

            // Build Header
            // TimeStamp
            mFileHandle << "[" << std::put_time(&now_tm, "%H:%M:%S") << "]";

            // File / Line Information
            mFileHandle << " [" << filePath.filename().string() << ":" << location.line() << "]"; 

            // LogLevel
            mFileHandle << " [" << logLevelString << "]";

            // Write message and flush the output
            mFileHandle << " " << message << std::endl;
        }
    }
}