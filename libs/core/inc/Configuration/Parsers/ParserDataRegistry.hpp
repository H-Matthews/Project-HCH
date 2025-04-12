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

    typedef std::map< std::string, std::any > FileToDataMap;
    typedef std::map< Parsers::ID, FileToDataMap > ParserDataContainer;

    class ParserDataRegistry
    {
      public:
        ParserDataRegistry( const ParserDataRegistry& obj ) = delete;

        static std::shared_ptr< ParserDataRegistry > instance();

        void registerParserID( Parsers::ID ID );

        void setParserData( Parsers::ID ID, const std::string& fileName, std::any parserData );

        // Returns empty std::any, if ID is not registered
        std::any getParserData( Parsers::ID ID, const std::string& fileName );

      private:
        ParserDataRegistry();

        // Static pointer to our object
        static std::shared_ptr< ParserDataRegistry > mRegistryInstance;

        ParserDataContainer mParserDataContainer;
        std::map< std::string, std::pair< Parsers::ID, std::string > > mFullyQualifedDataName;
    };

}
