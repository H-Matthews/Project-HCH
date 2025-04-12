#pragma once

#include "core/inc/Configuration/Parsers/Parser.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

namespace Core
{

    struct IniSection
    {
        std::string sectionName;
        std::map< std::string, std::string > keyValues;
        std::vector< std::shared_ptr< IniSection > > subSections;

        IniSection( const std::string name ) :
            sectionName( name ),
            keyValues(),
            subSections()
        {}
    };

    typedef std::map< std::string, std::shared_ptr< IniSection > > IniData;

    namespace IniToken
    {
        const char COMMENT[ 2 ] = { '#', ';' };
        const char SECTION_BRACKET_OPEN = '[';
        const char SECTION_BRACKET_END = ']';
        const char KEY_VALUE_ASSIGNMENT = '=';
        const char SUB_SECTION = '.';
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

        void parseFile( std::ifstream& fileStream, const std::string& fileName ) override;

      private:
        std::pair< std::string, bool > parseSection( const std::string& currentLine );
        std::pair< std::string, std::string > parseKeyValue( const std::string& currentLine );

        bool isIniTokenComment( const char token );
        bool isIniTokenSectionBracketOpen( const char token );
        bool isIniTokenSectionBracketEnd( const char token );
        bool isIniTokenKeyValueAssignment( const char token );
        bool isIniTokenSubSection( const char token );

        std::string trimSubSection( const std::string& currentSectionName );
        bool insertSection( std::pair< std::string, bool > sectionPair, IniData& dataStructure );
        void insertKeyValue( std::pair< std::string, std::string > keyValuePair, IniData& dataStructure );

      private:
        IniStatus mStatus;

        std::string mCurrentActiveSection;
    };
}