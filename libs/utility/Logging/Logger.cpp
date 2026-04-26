#include "utility/Logging/Logger.hpp"

#include "utility/Logging/LogRegistry.hpp"

// Builder implementation

Utility::Logger::Builder::Builder() :
    mLogger( std::make_shared< Logger >() )
{}

Utility::Logger::Builder& Utility::Logger::Builder::name( std::string n )
{
    mLogger->mLoggerName = std::move( n );
    return *this;
}

Utility::Logger::Builder& Utility::Logger::Builder::globalLogLevel( LogLevel level )
{
    mLogger->mGlobalLogLevel = level;
    return *this;
}

Utility::Logger::Builder& Utility::Logger::Builder::sinks( SinkList sinks )
{
    mLogger->mSinks = std::move( sinks );
    return *this;
}

std::shared_ptr< Utility::Logger > Utility::Logger::Builder::build()
{
    return mLogger;
}

Utility::Logger::Builder Utility::Logger::make()
{
    return Builder{};
}

Utility::Logger::Logger() :
    mSinks(),
    mLoggerName( "" ),
    mGlobalLogLevel( LogLevel::NONE )
{}

Utility::Logger::Logger( std::string loggerName, LogLevel level ) :
    mSinks(),
    mLoggerName( std::move( loggerName ) ),
    mGlobalLogLevel( level )
{}

Utility::Logger::Logger( std::string loggerName, std::shared_ptr< LogSink > sink, LogLevel level ) :
    mSinks( { sink } ),
    mLoggerName( std::move( loggerName ) ),
    mGlobalLogLevel( level )
{}

Utility::Logger::Logger( std::string loggerName, Logger::SinkList sinks, LogLevel level ) :
    mSinks( std::move( sinks ) ),
    mLoggerName( std::move( loggerName ) ),
    mGlobalLogLevel( level )
{}

void Utility::Logger::logDebug( std::string_view message, const std::source_location location )
{
    if constexpr (CAN_LOG_DEBUG)
        sinkIt( message, LogLevel::DEBUG, location );
}

void Utility::Logger::logInfo( std::string_view message, const std::source_location location )
{
    if constexpr (CAN_LOG_INFO)
        sinkIt( message, LogLevel::INFO, location );
}

void Utility::Logger::logWarn( std::string_view message, const std::source_location location )
{
    if constexpr (CAN_LOG_WARN)
        sinkIt( message, LogLevel::WARN, location );
}

void Utility::Logger::logError( std::string_view message, const std::source_location location )
{
    if constexpr (CAN_LOG_ERROR)
        sinkIt( message, LogLevel::ERROR, location );
}

void Utility::Logger::sinkIt( std::string_view message, LogLevel level, const std::source_location location )
{
    bool result = false;
    for (const auto& sink : mSinks)
    {
        // Ensure log levels are high enough to log
        result = shouldLog( level, sink->getSinkLogLevel() );
        if (result)
        {
            sink->sinkData( message, level, location );
        }
    }
}

void Utility::Logger::toggleGlobalLogger()
{
    mIsGlobalLogger = true;
}

// Ensures the log levels are high enough to log. Every log call goes through this check.
bool Utility::Logger::shouldLog( LogLevel level, LogLevel sinkLevel ) const
{
    bool canLog = false;

    // First, check to see if sink level is high enough
    if (sinkLevel != LogLevel::NONE && level >= sinkLevel)
    {
        canLog = true;
    }

    // If still false, check to see if global allows us to log
    if (!canLog && mGlobalLogLevel != LogLevel::NONE)
    {
        if (level >= mGlobalLogLevel)
            canLog = true;
    }

    return canLog;
}

void Utility::Logger::addSink( std::shared_ptr< LogSink > sink )
{
    // Probably should do more checks here for potential issues that I can't think of
    if (sink != nullptr)
        mSinks.push_back( sink );
}

void Utility::Logger::addSinkList( SinkList list )
{
    for (const auto& sink : list)
    {
        if (sink != nullptr)
            mSinks.push_back( sink );
    }
}

std::vector< Utility::LogSink* > Utility::Logger::getSinkReferences()
{
    std::vector< LogSink* > test;

    for (const auto& sink : mSinks)
    {
        test.push_back( sink.get() );
    }

    return test;
}

// NOT Apart of the class, but a friend
void Utility::createGlobalLogger()
{
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;

        auto globalConsoleLogger = std::make_shared< Utility::Logger >( "cLogger", Utility::LogLevel::DEBUG );
        globalConsoleLogger->toggleGlobalLogger();

        if (globalConsoleLogger)
            Utility::LogRegistry::instance()->registerLogger( globalConsoleLogger );
    }
}