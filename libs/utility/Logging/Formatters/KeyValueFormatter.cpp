#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include "utility/Time.hpp"

#include <iomanip>
#include <sstream>

Utility::KeyValueFormatter::KeyValueFormatter() :
    LogFormatter( "KeyValue" )
{}

static std::string jsonEscape( const std::string& s )
{
    std::string out;
    out.reserve( s.size() );
    for (char c : s)
    {
        if (c == '"')
            out += "\\\"";
        else if (c == '\\')
            out += "\\\\";
        else
            out += c;
    }
    return out;
}

std::string Utility::KeyValueFormatter::format(
    const std::string& message, LogLevel level, const std::source_location location )
{
    std::tm now_tm = Utility::getCurrentSystemTime();

    std::stringstream ss;
    ss << "{"
       << "\"file\":\"" << jsonEscape( location.file_name() ) << "\","
       << "\"line\":" << location.line() << ","
       << "\"function\":\"" << jsonEscape( location.function_name() ) << "\","
       << "\"message\":\"" << jsonEscape( message ) << "\","
       << "\"loglevel\":\"" << logLevelEnumToString( level ) << "\","
       << "\"time\":\"" << std::put_time( &now_tm, "%H:%M:%S" ) << "\""
       << "}";

    return ss.str();
}

std::string Utility::keysEnumToString( const Keys identifier )
{
    switch (identifier)
    {
        case Keys::FILE:
            return "file";
        case Keys::LINE:
            return "line";
        case Keys::FUNCTION:
            return "function";
        case Keys::TIME:
            return "time";
        case Keys::LOG_LEVEL:
            return "loglevel";
        case Keys::MESSAGE:
            return "message";
    }
    return "";
}
