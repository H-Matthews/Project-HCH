#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Utility
{
    class Logger
    {
        public:
            std::string log(const std::string& message);
    };
}

#endif