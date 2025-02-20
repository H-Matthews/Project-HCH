#pragma once

#include "utility/inc/Logging/FormatterI.hpp"


namespace Utility
{
    enum class Keys
    {
        FILE = 0,
        LINE,
        FUNCTION,
        TIME,
        LOG_LEVEL,
        MESSAGE
    };

    class KeyValueFormatter : public FormatterI
    {
        public:
            KeyValueFormatter();

            // From FormatterI
            std::string format(std::string message, LogLevel level, const std::source_location location) override;
    };

    const std::string keysEnumToString(const Keys identifier);
}