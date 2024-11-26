#pragma once

#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

#include <fstream>

namespace Utility
{
    class TextFileSink : public LogSinksI
    {
        static const std::string sinkIdentifier;

        public:
            TextFileSink() = delete;
            TextFileSink(const std::string& outputDirectory, 
                         const std::string& fileName, 
                         const std::string& logExtension,
                         LogLevel level = LogLevel::NONE);

            const std::string getFilePath() const;

            void sinkData(std::string_view message, LogLevel level, const std::source_location location) override;

        private:
            std::string mOutputDirectory;
            std::string mFileName;
            std::string mLogExtension;
            std::string mEntireFilePath;

            std::ofstream mFileHandle;
    };

    

    namespace Factory
    {
        // Convenience function
        // Creates Logger with the necessary Sink. Registers with LogRegistry
        std::shared_ptr< Utility::Logger > createTextFileLogger(const std::string& loggerName,
                                                                const std::string& outputDirectory,
                                                                const std::string& fileName, 
                                                                const std::string& logExtension,
                                                                LogLevel level = LogLevel::NONE);
    }
}