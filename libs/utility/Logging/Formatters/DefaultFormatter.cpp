#include "utility/Logging/Formatters/DefaultFormatter.hpp"

#include "utility/Time.hpp"

#include <iomanip>

Utility::DefaultFormatter::DefaultFormatter() :
    LogFormatter( "Default" )
{}

std::string Utility::DefaultFormatter::format(
    const std::string& message, LogLevel level, const std::source_location location )
{
    std::stringstream tempBuffer;

    std::tm now_tm = Utility::getCurrentSystemTime();

    // TIMESTAMP
    tempBuffer << "[" << std::put_time( &now_tm, "%H:%M:%S" ) << "]";

    // BEGIN FORMATTING

    // File / Line Information
    std::string_view fullPath = location.file_name();
    auto slash = fullPath.rfind( '/' );
    std::string_view filename = ( slash == std::string_view::npos ) ? fullPath : fullPath.substr( slash + 1 );
    tempBuffer << " [" << filename << ":" << location.line() << "]";

    // LogLevel
    const std::string logLevelString = logLevelEnumToString( level );
    tempBuffer << " [" << logLevelString << "]";

    // Message
    tempBuffer << " " << message;

    return tempBuffer.str();
}