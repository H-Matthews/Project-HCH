#include "utility/inc/Logging/Formatters/DefaultFormatter.hpp"

#include <chrono>
#include <iomanip>
#include <filesystem>

Utility::DefaultFormatter::DefaultFormatter() :
    LogFormatter( "Default" )
{}

std::string Utility::DefaultFormatter::format(
    std::string message, LogLevel level, const std::source_location location )
{
    std::stringstream tempBuffer;

    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t( now );
    std::tm now_tm = *std::localtime( &nowTime );

    // BEGIN FORMATTING

    // TIMESTAMP
    tempBuffer << "[" << std::put_time( &now_tm, "%H:%M:%S" ) << "]";

    // File / Line Information
    std::filesystem::path filePath( location.file_name() );
    tempBuffer << " [" << filePath.filename().string() << ":" << location.line() << "]";

    // LogLevel
    const std::string logLevelString = logLevelEnumToString( level );
    tempBuffer << " [" << logLevelString << "]";

    // Message
    tempBuffer << " " << message;

    return tempBuffer.str();
}