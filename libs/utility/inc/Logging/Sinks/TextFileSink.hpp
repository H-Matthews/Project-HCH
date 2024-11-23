#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

#include <fstream>

namespace Utility
{
    class TextFileSink : public LogSinksI
    {
        public:
            TextFileSink() = delete;
            TextFileSink(const std::string& outputDirectory, const std::string& fileName, const std::string& logExtension);

            const std::string getFilePath() const;

            void sinkData(std::string_view message, LogLevel level, const std::source_location location) override;

        private:
            std::string mOutputDirectory;
            std::string mFileName;
            std::string mLogExtension;
            std::string mEntireFilePath;

            std::ofstream mFileHandle;
    };
}