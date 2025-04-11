#pragma once

#include "core/inc/Configuration/Parsers/Parser.hpp"

#include <unordered_map>

namespace Core
{
    typedef std::unordered_map< std::string, std::string > KeyValueData;
    typedef std::unordered_map< std::string, KeyValueData > IniData;

    namespace IniToken
    {
        const char COMMENT[ 2 ] = { '#', ';' };
        const char SECTION_BRACKET_OPEN = '[';
        const char SECTION_BRACKET_END = ']';
        const char KEY_VALUE_ASSIGNMENT = '=';
    }

    class IniParser : public Parser
    {
      public:
        enum class IniStatus
        {
            READY_TO_PARSE_SECTION = 0,
            READY_TO_PARSE_KEY_VALUE
        };

        IniParser( const std::string parserIdentifierString );

        void parseFile( std::ifstream& fileStream ) override;

      private:
        void clearParserData();

        void parseSection( const std::string& currentLine );
        void parseKeyValue( const std::string& currentLine );

        bool isIniTokenComment( const char token );
        bool isIniTokenSectionBracketOpen( const char token );
        bool isIniTokenSectionBracketEnd( const char token );
        bool isIniTokenKeyValueAssignment( const char token );

      private:
        IniStatus mStatus;

        std::string mCurrentSection;
        IniData mData;
    };
}