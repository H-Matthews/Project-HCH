#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Utility
{
    class Logger;
    class LogSink;

    /**
     * This class is responsible for incrementally building a logger and its components
     */
    class LoggerBuilder
    {
      public:
        LoggerBuilder( Logger* logger );

        LoggerBuilder& name( const std::string& name );
        LoggerBuilder& globalLogLevel( const std::string& level );
        LoggerBuilder& sinks( std::vector< std::shared_ptr< LogSink > > sinks );

        ~LoggerBuilder()
        {}

        // Conversion Function
        operator Logger*()
        {
            return this->mLogger;
        }

      private:
        Logger* mLogger;
    };
}