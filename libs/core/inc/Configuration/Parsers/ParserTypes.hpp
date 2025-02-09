#pragma once

#include <string>

namespace Parsers
{
    enum class ID
    {
        NONE = 0,
        INI,
        JSON
    };

    const std::string parserEnumsToString(const Parsers::ID identifier);
}