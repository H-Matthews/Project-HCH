#pragma once

#include <string>

namespace Utility
{
    class ColorConsoleSink;

    class ColorConsoleSinkBuilder
    {
      public:
        ColorConsoleSinkBuilder( ColorConsoleSink* sink );

        ColorConsoleSinkBuilder& logLevel( const std::string& logLevel );

        ~ColorConsoleSinkBuilder()
        {}

        // Conversion Function
        operator ColorConsoleSink*()
        {
            return this->mSink;
        }

      private:
        ColorConsoleSink* mSink;
    };
}