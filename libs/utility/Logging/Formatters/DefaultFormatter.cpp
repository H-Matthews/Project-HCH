#include "utility/Logging/Formatters/DefaultFormatter.hpp"

#include "utility/Time.hpp"

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

    std::tm now_tm = Utility::getCurrentSystemTime();

    // TIMESTAMP
    tempBuffer << "[" << std::put_time( &now_tm, "%H:%M:%S" ) << "]";

    // BEGIN FORMATTING

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