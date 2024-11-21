#pragma once

#include "utility/inc/Logging/Sinks/LogSinksI.hpp"

#include <fstream>

namespace Utility
{
    class TextFileSink : public LogSinksI
    {
        public:
            TextFileSink() = delete;
            TextFileSink(const std::string& fileName, const std::string& outputDirectory, const std::string logExtension);

            void sinkData(std::string_view message, LogLevel level, const std::source_location location) override;

        private:
            std::string mFileName;
            std::string mOutputDirectory;
            std::string mLogExtension;
            std::string mEntireFilePath;

            std::ofstream mFileHandle;
    };
}