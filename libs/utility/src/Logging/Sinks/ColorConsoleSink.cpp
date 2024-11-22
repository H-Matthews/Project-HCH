#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/StringOperations.hpp"

#include <chrono>
#include <sstream>
#include <iostream>
#include <iomanip> // std::put_time
#include <filesystem>

namespace Utility
{
    ColorConsoleSink::ColorConsoleSink() :
        mOutputStream(std::cout)
    {
    }

    void ColorConsoleSink::sinkData(std::string_view message, LogLevel level, const std::source_location location)
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

    const std::string ColorConsoleSink::getColorCode(LogLevel level)
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

}