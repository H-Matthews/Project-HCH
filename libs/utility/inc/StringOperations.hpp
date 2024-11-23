
#pragma once

#include <sstream>

namespace Utility
{
    template <typename T>
    const std::string toString(const T& value);

    template <typename T>
    const std::string toString(const T& value)
    {
        std::stringstream sStream;
        sStream << value;
        return sStream.str();
    }
}