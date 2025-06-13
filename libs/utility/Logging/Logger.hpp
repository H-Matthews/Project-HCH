#pragma once

#include "utility/Logging/Sinks/LogSink.hpp"

#include <vector>

namespace Utility
{

// APP_DEBUG is defined by the CMake build
#ifdef APP_DEBUG
    constexpr bool CAN_LOG( true );
#else
    constexpr bool CAN_LOG( false );
#endif

    /**
     * Main Logger Class
     *
     * Loggers require Sinks to be attached in order to log a message
     * A logger with no defined sinks will not log messages
     */
    class Logger
    {
      public:
        typedef std::vector< std::shared_ptr< LogSink > > sinkList;

        Logger() = delete;
        Logger( const std::string& loggerName, Utility::LogLevel level = Utility::LogLevel::NONE );

        Logger( const std::string& loggerName, std::shared_ptr< Utility::LogSink > sink,
            Utility::LogLevel level = Utility::LogLevel::NONE );

        Logger( const std::string& loggerName, Utility::Logger::sinkList sinks,
            Utility::LogLevel level = Utility::LogLevel::NONE );

        void logDebug(
            std::string_view message, const std::source_location location = std::source_location::current() );
        void logInfo( std::string_view message, const std::source_location location = std::source_location::current() );
        void logWarn( std::string_view message, const std::source_location location = std::source_location::current() );
        void logError(
            std::string_view message, const std::source_location location = std::source_location::current() );

        bool shouldLog( LogLevel level, LogLevel sinkLevel ) const;

        inline void setGlobalLogLevel( LogLevel gLevel )
        {
            mGlobalLogLevel = gLevel;
        }
        inline LogLevel getGlobalLogLevel() const
        {
            return mGlobalLogLevel;
        }
        const std::string getGlobalLogLevelAsString() const;

        inline void setLoggerName( const std::string& name )
        {
            mLoggerName = name;
        }
        inline const std::string& getLoggerName() const
        {
            return mLoggerName;
        }

        // Inserts a single sink into mSinks
        void addSink( std::shared_ptr< LogSink > sink );

        // Insert multiple sinks into mSink
        void addSinkList( sinkList list );

        std::vector< LogSink* > getSinkReferences();

        inline bool getIsGlobalLogger() const
        {
            return mIsGlobalLogger;
        }

        // Define as a friend
        friend void createGlobalLogger();

      private:
        void sinkIt( std::string_view message, LogLevel level, const std::source_location location );
        void toggleGlobalLogger();

      private:
        std::vector< std::shared_ptr< LogSink > > mSinks;
        std::string mLoggerName;
        LogLevel mGlobalLogLevel;

        bool mIsGlobalLogger = false;
    };

    // Creates a Global Logger
    void createGlobalLogger();
}