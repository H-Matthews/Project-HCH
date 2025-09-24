#pragma once

#include "utility/Logging/Formatters/LogFormatter.hpp"

#include <filesystem>

namespace Utility
{

    class Logger;

    /**
     * Base class for implementing a Log output strategy
     *
     * Conceptually a "Sink" defines where our log message will be output
     * Each Sink has its own formatter that defines how the message is formatted
     */
    class LogSink
    {
      public:
        virtual void sinkData( std::string_view message, LogLevel level, const std::source_location location ) = 0;

        inline void setSinkLogLevel( LogLevel level );

        inline LogLevel getSinkLogLevel();

        inline void setFormatter( std::unique_ptr< LogFormatter > formatter );

        inline LogFormatter* getFormatter();

        virtual ~LogSink()
        {}

      protected:
        inline LogSink( const std::string& sinkIdentifier ) :
            mFormatter( nullptr ),
            mSinkIdentifier( sinkIdentifier ),
            mLevel( LogLevel::NONE ) {};

        inline LogSink( const std::string& sinkIdentifier, std::unique_ptr< LogFormatter > formatter,
            LogLevel level = LogLevel::NONE ) :
            mFormatter( std::move( formatter ) ),
            mSinkIdentifier( sinkIdentifier ),
            mLevel( level ) {};

        // Pointer to the Format type
        std::unique_ptr< LogFormatter > mFormatter;

        // Each Sink has a LogLevel.
        // If its set to NONE, we use the global logger level
        const std::string mSinkIdentifier;
        LogLevel mLevel;
    };

    void LogSink::setSinkLogLevel( LogLevel level )
    {
        mLevel = level;
    }

    LogLevel LogSink::getSinkLogLevel()
    {
        return mLevel;
    }

    void LogSink::setFormatter( std::unique_ptr< LogFormatter > formatter )
    {
        mFormatter.swap( formatter );
    }

    LogFormatter* LogSink::getFormatter()
    {
        return mFormatter.get();
    }
}