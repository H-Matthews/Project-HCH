#pragma once

#include "core/inc/Configuration/Parsers/ParserData.hpp"

#include <fstream>
#include <string>
#include <memory>

namespace Core
{

    class Parser
    {
      public:
        inline Parser( const std::string parserIdentifierString, std::unique_ptr< ParserData > parserData ) :
            mParserIdentifierString( parserIdentifierString ),
            mParserData( std::move( parserData ) )
        {}

        inline ParserData* getParserData()
        {
            return mParserData.get();
        };

        virtual void parseFile( std::ifstream& fileStream ) = 0;

        virtual ~Parser()
        {}

      protected:
        std::string mParserIdentifierString;

        std::unique_ptr< ParserData > mParserData;
    };
}