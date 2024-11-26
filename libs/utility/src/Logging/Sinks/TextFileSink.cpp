#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <iostream>

const std::string Utility::TextFileSink::sinkIdentifier = "TextFileSink";

Utility::TextFileSink::TextFileSink(const std::string& outputDirectory, 
                            const std::string& fileName, 
                            const std::string& logExtension,
                            LogLevel level) :
    LogSinksI(sinkIdentifier, level),
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

const std::string Utility::TextFileSink::getFilePath() const
{
    return mEntireFilePath;
}

void Utility::TextFileSink::sinkData(std::string_view message, LogLevel level, const std::source_location location)
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

std::shared_ptr< Utility::Logger > Utility::Factory::createTextFileLogger(const std::string& loggerName,
                                                        const std::string& outputDirectory,
                                                        const std::string& fileName, 
                                                        const std::string& logExtension,
                                                        LogLevel level)
{
    auto textFileSink = std::make_shared< Utility::TextFileSink >(outputDirectory, fileName, logExtension, level);

    auto logger = std::make_shared< Utility::Logger >(loggerName, textFileSink);
    Utility::LogRegistry::instance()->registerLogger(logger);

    return logger;
}
