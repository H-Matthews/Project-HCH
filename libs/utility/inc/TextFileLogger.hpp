#pragma once

#include "utility/inc/LoggerI.hpp"

#include <fstream>

namespace Utility
{
    class TextFileLogger : public LoggerI
    {
        public:
            TextFileLogger(const std::string& fileName, const std::string& outputDirPath);
            void log(const std::string& message, LogLevel level, const char* file, int line) override;

        private:
            std::string mOutputDirPath;
            const std::string mLogExtension;

            std::string mFileName;
            std::ofstream mFileHandle;
    };
}