#pragma once

#include "utility/Logging/Logger.hpp"

#include <fstream>

namespace Utility
{
    /**
     * TextFileSink writes output to a specific file
     */
    class TextFileSink : public LogSink
    {
        static const std::string sinkIdentifier;

      public:
        TextFileSink() = delete;
        TextFileSink( const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension,
            LogLevel level = LogLevel::NONE );

        const std::string getFilePath() const;

        void sinkData( std::string_view message, LogLevel level, const std::source_location location ) override;

        ~TextFileSink()
        {}

      private:
        std::string mOutputDirectory;
        std::string mFileName;
        std::string mLogExtension;
        std::string mEntireFilePath;

        std::ofstream mFileHandle;
    };

    // Convenience function
    // Creates Logger with the necessary Sink. Registers with LogRegistry
    std::shared_ptr< Utility::Logger > createTextFileLogger( const std::string& loggerName,
        const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension,
        LogLevel level = LogLevel::NONE );
}