#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

#include <chrono>
#include <iostream>


const std::string Utility::ColorConsoleSink::sinkIdentifier = "ColorConsoleSink";

Utility::ColorConsoleSink::ColorConsoleSink(LogLevel level) :
    LogSinksI(sinkIdentifier, level),
    mOutputStream(std::cout)
{
}

void Utility::ColorConsoleSink::sinkData(std::string_view message, LogLevel level, const std::source_location location)
{
    // Get Time Stamp
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&nowTime);

    std::string colorCode = getColorCode(level);
    std::filesystem::path filePath(location.file_name());
    const std::string logLevelString = logLevelEnumToString(level);

    // Build Header
    // TimeStamp
    mOutputStream << colorCode << "[" << std::put_time(&now_tm, "%H:%M:%S") << "]";

    // File / Line Information
    mOutputStream << " [" << filePath.filename().string() << ":" << location.line() << "]"; 

    // LogLevel
    mOutputStream << " [" << logLevelString << "]";

    // Write message, put text color back to default, and flush the output
    mOutputStream << " " << message << mDefaultColorCode << std::endl;
}

const std::string Utility::ColorConsoleSink::getColorCode(LogLevel level) const
{
    std::stringstream colorCode;
    switch (level)
    {
        // ANSI Color Codes
        case LogLevel::DEBUG:
        {
            colorCode << mDebugColorCode;      // Blue 
            break;
        }
        case LogLevel::INFO:
        {
            colorCode << mInfoColorCode;       // Green
            break;
        }
        case LogLevel::WARN:
        {
            colorCode << mWarningColorCode;    // Yellow 
            break;
        }
        case LogLevel::ERROR:
        {
            colorCode << mErrorColorCode;      // Red
            break;
        }
        default:
        {
            colorCode << mDefaultColorCode;    // Reset
            break;
        }
    }

    return colorCode.str();
}

// Convenience function to create a logger that has the ColorConsoleSink
std::shared_ptr< Utility::Logger > Utility::Factory::createColorConsoleLogger(const std::string& loggerName, LogLevel level)
{
    auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >(level);

    auto logger = std::make_shared< Utility::Logger >(loggerName, colorConsoleSink);
    Utility::LogRegistry::instance()->registerLogger(logger);

    return logger;
}