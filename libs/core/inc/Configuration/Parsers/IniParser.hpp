#pragma once

#include "core/inc/Configuration/Parsers/Parser.hpp"

namespace Core
{

    typedef std::map< std::string, std::pair< std::string, std::string > > IniData;

    class IniParser : public Parser
    {
      public:
        IniParser( const std::string parserIdentifierString );

        void parseFile( std::ifstream& fileStream ) override;
    };
}