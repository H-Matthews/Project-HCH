#include "core/inc/Configuration/Parsers/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, Parsers::ID::INI )
{}

void Core::IniParser::parseFile( std::ifstream& fileStream )
{
    IniData iniData;
    iniData.insert( { "Test", std::make_pair( "Key", "Value" ) } );

    Core::ParserDataRegistry::instance()->setParserData( mParserID, std::any( iniData ) );

    return;
}