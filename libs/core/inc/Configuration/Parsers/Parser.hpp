#pragma once

#include "core/inc/Configuration/Parsers/ParserTypes.hpp"
#include "core/inc/Configuration/Parsers/ParserDataRegistry.hpp"

#include <fstream>
#include <string>
#include <memory>

namespace Core
{

    class Parser
    {
      public:
        inline Parser( const std::string parserIdentifierString, Parsers::ID parserID ) :
            mParserIdentifierString( parserIdentifierString ),
            mParserID( parserID )
        {
            Core::ParserDataRegistry::instance()->registerParserID( mParserID );
        }

        virtual void parseFile( std::ifstream& fileStream, const std::string& fileName ) = 0;

        virtual ~Parser()
        {}

      protected:
        std::string mParserIdentifierString;
        Parsers::ID mParserID;
    };
}