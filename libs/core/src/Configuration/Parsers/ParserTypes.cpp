#include "core/inc/Configuration/Parsers/ParserTypes.hpp"

// IMPORTANT The string MUST match the file extension EXACTLY
const std::string Parsers::parserEnumsToString(const Parsers::ID identifier)
{
    std::string enumString;

    switch(identifier)
    {
        case Parsers::ID::NONE:
        {
            enumString = "None";
            break;
        }
        case Parsers::ID::INI:
        {
            enumString = ".ini";
            break;
        }
        case Parsers::ID::JSON:
        {
            enumString = ".json";
            break;
        }
    }

    return enumString;
}