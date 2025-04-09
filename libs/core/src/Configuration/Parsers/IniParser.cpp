#include "core/inc/Configuration/Parsers/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, std::make_unique< IniParserData >() )
{}

void Core::IniParser::parseFile( std::ifstream& fileStream )
{

    IniData testData;
    testData.insert( { "Test", std::make_pair( "Key", "Value" ) } );

    mParserData->setData( std::any( testData ) );

    return;
}