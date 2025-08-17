#include "utility/Logging/Builder/ColorConsoleSinkBuilder.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogLevel.hpp"

Utility::ColorConsoleSinkBuilder::ColorConsoleSinkBuilder( Utility::ColorConsoleSink* sink ) :
    mSink( sink )
{}

Utility::ColorConsoleSinkBuilder& Utility::ColorConsoleSinkBuilder::logLevel( const std::string& logLevel )
{
    Utility::LogLevel localLogLevel = stringToLogLevelEnum( logLevel );

    mSink->mLevel = localLogLevel;

    return *this;
}