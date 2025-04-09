#include "core/inc/Configuration/Parsers/JsonParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::JsonParser::JsonParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, Parsers::ID::JSON )
{}

void Core::JsonParser::parseFile( std::ifstream& fileStream )
{
    return;
}