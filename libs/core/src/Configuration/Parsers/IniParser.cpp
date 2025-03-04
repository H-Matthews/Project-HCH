#include "core/inc/Configuration/Parsers/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString )
{}

void Core::IniParser::parseFile( std::ifstream& fileStream )
{
    return;
}