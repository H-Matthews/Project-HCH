#pragma once

#include "utility/Logging/Logger.hpp"

#include "utility/Logging/Builder/TextFileSinkBuilder.hpp"

#include <fstream>

namespace Utility
{
    /**
     * TextFileSink writes output to a specific file
     */
    class TextFileSink : public LogSink
    {
        static const std::string SINK_IDENTIFIER;

      public:
        TextFileSink();
        TextFileSink( const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension,
            LogLevel level = LogLevel::NONE );

        std::string getFilePath() const;

        void sinkData( std::string_view message, LogLevel level, const std::source_location location ) override;

        void openFile();

        ~TextFileSink()
        {}

        inline static TextFileSinkBuilder build();
        friend class TextFileSinkBuilder;

      private:
        std::string mOutputDirectory;
        std::string mFileName;
        std::string mLogExtension;
        std::string mEntireFilePath;

        std::ofstream mFileHandle;
    };

    Utility::TextFileSinkBuilder Utility::TextFileSink::build()
    {
        TextFileSink* sink = new TextFileSink();

        return Utility::TextFileSinkBuilder( sink );
    }

    // Convenience function
    // Creates Logger with the necessary Sink. Registers with LogRegistry
    std::shared_ptr< Utility::Logger > createTextFileLogger( const std::string& loggerName,
        const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension,
        LogLevel level = LogLevel::NONE );
}