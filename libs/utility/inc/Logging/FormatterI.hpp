#pragma once

#include <string>
#include <source_location>
#include "utility/inc/Logging/LogLevel.hpp"

namespace Utility
{
    class FormatterI
    {
        public:
            FormatterI(const std::string& name) :
                mName(name) {}

            virtual std::string format(std::string message, LogLevel level, const std::source_location location) = 0;

        private:
            std::string mName;
    };
}