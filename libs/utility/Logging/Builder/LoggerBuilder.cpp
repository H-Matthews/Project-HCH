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

Utility::LoggerBuilder& Utility::LoggerBuilder::logLevel( const std::string& level )
{
    LogLevel globalLogLevel = stringToLogLevelEnum( level );

    mLogger->mGlobalLogLevel = globalLogLevel;

    return *this;
}

Utility::LoggerBuilder& Utility::LoggerBuilder::sinks( const std::vector< std::string >& sinkIdentifiers )
{
    // This will require some work

    return *this;
}