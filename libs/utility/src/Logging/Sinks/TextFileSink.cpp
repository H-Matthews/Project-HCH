#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Formatters/KeyValueFormatter.hpp"

#include <sstream>

const std::string Utility::TextFileSink::sinkIdentifier = "TextFileSink";

Utility::TextFileSink::TextFileSink(
    const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension, LogLevel level ) :
    LogSink( sinkIdentifier, std::make_unique< KeyValueFormatter >(), level ),
    mOutputDirectory( outputDirectory ),
    mFileName( fileName ),
    mLogExtension( logExtension )
{
    // Build Path
    std::stringstream filePath;
    filePath << outputDirectory << "/";
    filePath << fileName << logExtension;

    // Open File
    mFileHandle.open( filePath.str(), std::ios::app );

    // Store path
    mEntireFilePath = filePath.str();
}

const std::string Utility::TextFileSink::getFilePath() const
{
    return mEntireFilePath;
}

void Utility::TextFileSink::sinkData( std::string_view message, LogLevel level, const std::source_location location )
{
    if ( mFormatter )
    {
        std::string formattedMessage = mFormatter->format( std::string( message ), level, location );

        if ( mFileHandle.is_open() )
            mFileHandle << formattedMessage << std::endl;
    }
    else
    {
        LogRegistry::instance()->getGlobalLogger()->logError(
            "Logger: " + LogSink::mSinkIdentifier + " DOES NOT have a formatter" );
    }
}

std::shared_ptr< Utility::Logger > Utility::createTextFileLogger( const std::string& loggerName,
    const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension, LogLevel level )
{
    auto textFileSink = std::make_shared< Utility::TextFileSink >( outputDirectory, fileName, logExtension, level );

    auto logger = std::make_shared< Utility::Logger >( loggerName, textFileSink );
    Utility::LogRegistry::instance()->registerLogger( logger );

    return logger;
}
