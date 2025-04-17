#include "core/inc/Configuration/Parsers/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/StringOperations.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, Parsers::ID::INI ),
    mStatus( Core::IniParser::IniStatus::READY_TO_PARSE_SECTION ),
    mCurrentActiveSection()
{}

bool Core::IniParser::parseFile( std::ifstream& fileStream, const std::string& fileName )
{
    if ( !fileStream.is_open() )
        return false;

    // Parser is used to parse multiple files, so we must ensure the state is
    // clear when beginning to parse a file
    mCurrentActiveSection.clear();
    mStatus = IniStatus::READY_TO_PARSE_SECTION;

    IniData iniDataStructure;

    std::string currentLine;
    while ( std::getline( fileStream, currentLine ) )
    {
        // IF, its an empty line, then go next
        if ( currentLine.empty() )
            continue;

        currentLine = Utility::trimWhiteSpace( currentLine );

        const char firstChar = currentLine[ 0 ];
        if ( isIniTokenComment( firstChar ) )
        {
            continue;
        }
        else if ( isIniTokenSectionBracketOpen( firstChar ) )
        {
            if ( mStatus == IniStatus::READY_TO_PARSE_KEY_VALUE )
                mStatus = IniStatus::READY_TO_PARSE_SECTION;

            if ( mStatus == IniStatus::READY_TO_PARSE_SECTION )
            {
                auto sectionPair = parseSection( currentLine );

                if ( insertSection( sectionPair, iniDataStructure ) )
                    mStatus = IniStatus::READY_TO_PARSE_KEY_VALUE;
            }
        }
        else
        {
            if ( mStatus == IniStatus::READY_TO_PARSE_KEY_VALUE )
            {
                auto keyValuePair = parseKeyValue( currentLine );
                insertKeyValue( keyValuePair, iniDataStructure );
            }
        }
    }

    // Save Ini File Data Entry
    Core::ParserDataRegistry::instance()->setParserData( mParserID, fileName, std::any( iniDataStructure ) );

    return true;
}

std::pair< std::string, bool > Core::IniParser::parseSection( const std::string& currentLine )
{
    bool isSubSection = false;
    std::string currentSection;

    // Index starts at 1 because of starting '[' character
    for ( long unsigned int i = 1; i < currentLine.size(); i++ )
    {
        if ( isIniTokenSubSection( currentLine[ i ] ) )
            isSubSection = true;

        if ( !( isIniTokenSectionBracketEnd( currentLine[ i ] ) ) )
        {
            currentSection += currentLine[ i ];
        }
        else
        {
            break;
        }
    }

    currentSection = Utility::trimWhiteSpace( currentSection );

    // Set this var for convenience when inserting key values
    mCurrentActiveSection = currentSection;

    return std::make_pair( currentSection, isSubSection );
}

std::pair< std::string, std::string > Core::IniParser::parseKeyValue( const std::string& currentLine )
{
    std::string key;
    std::string value;

    size_t position = currentLine.find( Core::IniToken::KEY_VALUE_ASSIGNMENT );
    if ( position == std::string::npos )
        return std::make_pair( std::string( "" ), std::string( "" ) );

    // Two operations, 1. gets substring 2. trims whitespace
    key = Utility::trimWhiteSpace( currentLine.substr( 0, position ) );
    value = Utility::trimWhiteSpace( currentLine.substr( position + 1, currentLine.size() ) );

    return std::make_pair( key, value );
}

bool Core::IniParser::insertSection( std::pair< std::string, bool > sectionPair, IniData& dataStructure )
{
    // Determine if this is a supposed to be a Subsection
    if ( sectionPair.second )
    {
        auto it = dataStructure.find( trimSubSection( sectionPair.first ) );
        if ( it == dataStructure.end() )
        {
            // Log here
            return false;
        }

        auto subSection = std::make_shared< IniSection >( sectionPair.first );
        dataStructure.insert( std::make_pair( sectionPair.first, subSection ) );

        // Insert as a SubSection (Not sure if this is needed. May come in handy later)
        it->second->subSections.push_back( subSection );
    }
    else
    {
        auto subSection = std::make_shared< IniSection >( sectionPair.first );
        dataStructure.insert( std::make_pair( sectionPair.first, subSection ) );
    }

    return true;
}

void Core::IniParser::insertKeyValue( std::pair< std::string, std::string > keyValuePair, IniData& dataStructure )
{
    auto it = dataStructure.find( mCurrentActiveSection );
    if ( it == dataStructure.end() )
    {
        // Log here
        return;
    }

    it->second->keyValues.insert( keyValuePair );

    return;
}

std::string Core::IniParser::trimSubSection( const std::string& currentSectionName )
{
    // Grab last position of substring
    size_t position = currentSectionName.rfind( Core::IniToken::SUB_SECTION );
    if ( position == std::string::npos )
        return std::string{};

    return currentSectionName.substr( 0, position );
}

bool Core::IniParser::isIniTokenComment( const char token )
{
    for ( const char& iniComment : Core::IniToken::COMMENT )
    {
        if ( token == iniComment )
            return true;
    }

    return false;
}

bool Core::IniParser::isIniTokenSectionBracketOpen( const char token )
{
    if ( token == Core::IniToken::SECTION_BRACKET_OPEN )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenSectionBracketEnd( const char token )
{
    if ( token == Core::IniToken::SECTION_BRACKET_END )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenKeyValueAssignment( const char token )
{
    if ( token == Core::IniToken::KEY_VALUE_ASSIGNMENT )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenSubSection( const char token )
{
    if ( token == Core::IniToken::SUB_SECTION )
        return true;

    return false;
}