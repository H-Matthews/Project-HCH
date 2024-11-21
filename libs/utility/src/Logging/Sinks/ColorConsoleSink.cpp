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

        // Get Correct ColorCode based on logLevel
        std::string colorCode = getColorCode(level);

        // Get LogLevel as String
        const std::string logLevelString = logLevelEnumToString(level);

        // Build Header
        mOutputStream << colorCode << "[" << std::put_time(&now_tm, "%H:%M:%S") << "] "
                  << "[" << logLevelString << "]";

        // Parse the file path for JUST the file NAME
        std::filesystem::path filePath(location.file_name());

        mOutputStream << " [" << filePath.filename().string() << ":" << location.line() << "]"; 

        // Write message, put text color back to default
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