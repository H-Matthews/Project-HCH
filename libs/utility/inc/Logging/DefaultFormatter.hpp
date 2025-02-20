#pragma once

#include "utility/inc/Logging/FormatterI.hpp"


namespace Utility
{
    class DeafaultFormatter : public FormatterI
    {
        public:
            DeafaultFormatter();

            // From FormatterI
            std::string format(std::string message, LogLevel level, const std::source_location location) override;
    };
}