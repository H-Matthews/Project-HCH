#pragma once

#include "utility/inc/LoggerI.hpp"

#include <fstream>

namespace Utility
{
    class TextFileLogger : public LoggerI
    {
        public: 
            TextFileLogger(const std::string& fileName);
            TextFileLogger();

            void initialize(const std::string& fileName);

            void log(const std::string& message, LogLevel level, const char* file, int line) override;

        private:
            void establishLogger();

        private:
            std::string mFileName;
            std::ofstream mFileHandle;
            const std::string logExtension = ".log";
    };
}