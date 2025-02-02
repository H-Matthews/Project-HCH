#include "core/inc/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::IniParser::IniParser(const std::string& nameID, const std::string& fileExtension) :
    ParserI(nameID, fileExtension)
{
}

bool Core::IniParser::parseFile(std::ifstream& fileStream)
{
    bool returnStatus = false;

    if( !(fileStream.is_open()))
        return returnStatus;
}