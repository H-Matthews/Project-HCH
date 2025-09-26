#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Formatters/DefaultFormatter.hpp"

#include <iostream>

const std::string Utility::ColorConsoleSink::sinkIdentifier = "ColorConsoleSink";

Utility::ColorConsoleSink::ColorConsoleSink() :
    LogSink( sinkIdentifier ),
    mOutputStream( std::cout )
{}

void Utility::ColorConsoleSink::sinkData(
    std::string_view message, LogLevel level, const std::source_location location )
{
    if ( mFormatter )
    {
        std::string formattedMessage = mFormatter->format( std::string( message ), level, location );

        const std::string colorCode = getColorCode( level );
        insertColorCodes( formattedMessage, colorCode );

        // Write Log
        mOutputStream << formattedMessage << std::endl;
    }

    return;
}

void Utility::ColorConsoleSink::insertColorCodes( std::string& message, std::string colorCode )
{
    // Insert color codes into message
    message.insert( 0, colorCode );
    message.insert( message.size(), mDefaultColorCode );
}

const std::string Utility::ColorConsoleSink::getColorCode( LogLevel level ) const
{
    std::string colorCode;
    switch ( level )
    {
        // ANSI Color Codes
        case LogLevel::DEBUG:
        {
            colorCode = mDebugColorCode; // Blue
            break;
        }
        case LogLevel::INFO:
        {
            colorCode = mInfoColorCode; // Green
            break;
        }
        case LogLevel::WARN:
        {
            colorCode = mWarningColorCode; // Yellow
            break;
        }
        case LogLevel::ERROR:
        {
            colorCode = mErrorColorCode; // Red
            break;
        }
        default:
        {
            colorCode = mDefaultColorCode; // Reset
            break;
        }
    }

    return colorCode;
}

// Convenience function to create a logger that has the ColorConsoleSink
// std::shared_ptr< Utility::Logger > Utility::createColorConsoleLogger( const std::string& loggerName, LogLevel level )
// {
//     auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >( level );

//     auto logger = std::make_shared< Utility::Logger >( loggerName, colorConsoleSink );
//     Utility::LogRegistry::instance()->registerLogger( logger );

//     return logger;
// }