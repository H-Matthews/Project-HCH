#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include "utility\Time.hpp"

#include <iomanip>
#include <iostream>
#include <chrono>

Utility::KeyValueFormatter::KeyValueFormatter() :
    LogFormatter( "KeyValue" )
{}

std::string Utility::KeyValueFormatter::format(
    std::string message, LogLevel level, const std::source_location location )
{
    std::stringstream tempBuffer;

    // BEGIN FORMATTING

    tempBuffer << "{ \n\t";

    // Key: File
    tempBuffer << "\"" << keysEnumToString( Keys::FILE ) << "\"" << ":" << location.file_name() << "\n\t";

    // Key: Line
    tempBuffer << "\"" << keysEnumToString( Keys::LINE ) << "\"" << ":" << location.line() << "\n\t";

    // Key: Function
    tempBuffer << "\"" << keysEnumToString( Keys::FUNCTION ) << "\"" << ":" << location.function_name() << "\n\t";

    // Key: Message
    tempBuffer << "\"" << keysEnumToString( Keys::MESSAGE ) << "\"" << ":" << message << "\n\t";

    // Key: Log Level
    tempBuffer << "\"" << keysEnumToString( Keys::LOG_LEVEL ) << "\"" << ":" << logLevelEnumToString( level ) << "\n\t";

    std::optional< std::shared_ptr< struct tm > > currentTime = Utility::getCurrentSystemTime();
    if (currentTime.has_value())
    {
        // Key: Time
        tempBuffer << "\"" << keysEnumToString( Keys::TIME ) << "\"" << ":"
                   << std::put_time( ( currentTime.value().get() ), "%H:%M:%S" ) << "\n";
    }

    // END
    tempBuffer << "},";

    return tempBuffer.str();
}

const std::string Utility::keysEnumToString( const Keys identifier )
{
    std::string buffer;

    switch (identifier)
    {
        case Keys::FILE:
        {
            buffer = "file";
            break;
        }
        case Keys::LINE:
        {
            buffer = "line";
            break;
        }
        case Keys::FUNCTION:
        {
            buffer = "function";
            break;
        }
        case Keys::TIME:
        {
            buffer = "time";
            break;
        }
        case Keys::LOG_LEVEL:
        {
            buffer = "loglevel";
            break;
        }
        case Keys::MESSAGE:
        {
            buffer = "message";
            break;
        }
    }

    return buffer;
}