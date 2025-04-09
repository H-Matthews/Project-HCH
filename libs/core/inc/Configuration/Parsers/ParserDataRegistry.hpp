#pragma once

#include "core/inc/Configuration/Parsers/ParserTypes.hpp"

#include <memory>
#include <map>
#include <any>

namespace Core
{

    /**
     * ParserDataRegistry is a Singleton that defines a registry of data
     */

    class ParserDataRegistry
    {
      public:
        // Delete the Copy Constructor
        ParserDataRegistry( const ParserDataRegistry& obj ) = delete;

        static std::shared_ptr< ParserDataRegistry > instance();

        void registerParserID( Parsers::ID ID );

        void setParserData( Parsers::ID ID, std::any parserData );

        // Returns empty std::any, if ID is not registered
        std::any getParserData( Parsers::ID ID );

      private:
        ParserDataRegistry();

        // Static pointer to our object
        static std::shared_ptr< ParserDataRegistry > mRegistryInstance;

        std::map< Parsers::ID, std::any > mParserData;
    };

}
