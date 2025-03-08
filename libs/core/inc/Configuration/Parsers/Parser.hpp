#pragma once

#include <fstream>
#include <string>

namespace Core
{

    class Parser
    {
      public:
        inline Parser( const std::string parserIdentifierString ) :
            mParserIdentifierString( parserIdentifierString )
        {}

        virtual void parseFile( std::ifstream& fileStream ) = 0;

        virtual ~Parser()
        {}

      private:
        std::string mParserIdentifierString;
    };
}