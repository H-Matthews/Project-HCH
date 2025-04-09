#include "core/inc/Configuration/Parsers/JsonParser.hpp"
#include "core/inc/Configuration/Parsers/JsonParserData.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::JsonParser::JsonParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, std::make_unique< JsonParserData >() )
{}

void Core::JsonParser::parseFile( std::ifstream& fileStream )
{
    return;
}