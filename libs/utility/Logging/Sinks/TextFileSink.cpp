#include "utility/Logging/Sinks/TextFileSink.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Formatters/KeyValueFormatter.hpp"

#include <sstream>

const std::string Utility::TextFileSink::SINK_IDENTIFIER = "TextFileSink";

// Builder implementation

Utility::TextFileSink::Builder::Builder() :
    mSink( std::make_shared< TextFileSink >() )
{}

Utility::TextFileSink::Builder& Utility::TextFileSink::Builder::outputDirectory( std::string outputDirectory )
{
    mSink->mOutputDirectory = std::move( outputDirectory );
    return *this;
}

Utility::TextFileSink::Builder& Utility::TextFileSink::Builder::fileName( std::string fileName )
{
    mSink->mFileName = std::move( fileName );
    return *this;
}

Utility::TextFileSink::Builder& Utility::TextFileSink::Builder::logExtension( std::string logExtension )
{
    mSink->mLogExtension = std::move( logExtension );
    return *this;
}

Utility::TextFileSink::Builder& Utility::TextFileSink::Builder::logLevel( const std::string& level )
{
    mSink->setSinkLogLevel( stringToLogLevelEnum( level ) );
    return *this;
}

Utility::TextFileSink::Builder& Utility::TextFileSink::Builder::formatter( std::unique_ptr< LogFormatter > f )
{
    mSink->setFormatter( std::move( f ) );
    return *this;
}

std::shared_ptr< Utility::TextFileSink > Utility::TextFileSink::Builder::build()
{
    mSink->openFile();
    return mSink;
}

Utility::TextFileSink::Builder Utility::TextFileSink::make()
{
    return Builder{};
}

Utility::TextFileSink::TextFileSink() :
    LogSink( SINK_IDENTIFIER ),
    mOutputDirectory( "" ),
    mFileName( "" ),
    mLogExtension( "" )
{}

Utility::TextFileSink::TextFileSink(
    const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension, LogLevel level ) :
    LogSink( SINK_IDENTIFIER, std::make_unique< KeyValueFormatter >(), level ),
    mOutputDirectory( outputDirectory ),
    mFileName( fileName ),
    mLogExtension( logExtension )
{
    openFile();
}

const std::string Utility::TextFileSink::getFilePath() const
{
    return mEntireFilePath;
}

void Utility::TextFileSink::sinkData( std::string_view message, LogLevel level, const std::source_location location )
{
    if (mFormatter)
    {
        std::string formattedMessage = mFormatter->format( std::string( message ), level, location );

        if (mFileHandle.is_open())
            mFileHandle << formattedMessage << '\n';
    }
    else
    {
        LogRegistry::instance()->getGlobalLogger()->logError(
            "Logger: " + LogSink::mSinkIdentifier + " DOES NOT have a formatter" );
    }
}

void Utility::TextFileSink::openFile()
{
    // Build Path
    std::stringstream filePath;
    filePath << mOutputDirectory << "/";
    filePath << mFileName << mLogExtension;

    // Open File
    mFileHandle.open( filePath.str(), std::ios::app );

    // Store path
    mEntireFilePath = filePath.str();
}

std::shared_ptr< Utility::Logger > Utility::createTextFileLogger( const std::string& loggerName,
    const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension, LogLevel level )
{
    auto textFileSink = std::make_shared< Utility::TextFileSink >( outputDirectory, fileName, logExtension, level );

    auto logger = std::make_shared< Utility::Logger >( loggerName, textFileSink );
    Utility::LogRegistry::instance()->registerLogger( logger );

    return logger;
}
