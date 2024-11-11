#pragma once

#include "utility/inc/LoggerI.hpp"

#include <functional>   // Hash
#include <map>
#include <memory>

namespace Utility
{
    class TextFileLogger : public LoggerI
    {
        public:
            static void establishLogger(const std::string& loggerName);
            static std::shared_ptr<TextFileLogger> getLogger(const std::string& loggerName);

            void log(const std::string& message, LogLevel level, const char* file, int line) override;

        public:
            // Key is hashed Logger Name
            static std::map< std::size_t, std::shared_ptr<TextFileLogger> > mLoggers;
    };
}