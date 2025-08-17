#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Utility
{
    class Logger;

    /**
     * This class is responsible for incrementally building a logger and its components
     */
    class LoggerBuilder
    {
      public:
        LoggerBuilder( Logger* logger );

        LoggerBuilder& name( const std::string& name );
        LoggerBuilder& logLevel( const std::string& level );
        LoggerBuilder& sinks( const std::vector< std::string >& sinkIdentifiers );

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