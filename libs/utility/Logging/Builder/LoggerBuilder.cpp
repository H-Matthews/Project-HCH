#include "utility/Logging/Builder/LoggerBuilder.hpp"

#include "utility/Logging/Logger.hpp"

Utility::LoggerBuilder::LoggerBuilder( Logger* logger ) :
    mLogger( logger )
{}

Utility::LoggerBuilder& Utility::LoggerBuilder::name( const std::string& name )
{
    mLogger->mLoggerName = name;

    return *this;
}

Utility::LoggerBuilder& Utility::LoggerBuilder::globalLogLevel( const std::string& level )
{
    LogLevel globalLogLevel = stringToLogLevelEnum( level );

    mLogger->mGlobalLogLevel = globalLogLevel;

    return *this;
}

Utility::LoggerBuilder& Utility::LoggerBuilder::sinks( std::vector< std::shared_ptr< LogSink > > sinks )
{
    mLogger->mSinks = sinks;

    return *this;
}