#include "core/inc/Configuration/Parsers/IniParser.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, Parsers::ID::INI ),
    mStatus( Core::IniParser::IniStatus::READY_TO_PARSE_SECTION ),
    mCurrentSection(),
    mData()
{}

void Core::IniParser::clearParserData()
{
    mData.clear();
    mCurrentSection.clear();
    mStatus = IniStatus::READY_TO_PARSE_SECTION;
}

void Core::IniParser::parseFile( std::ifstream& fileStream )
{
    // Ensure we have a clean state (This parser is used to parse multiple ini files)
    if ( mData.size() > 0 )
        clearParserData();

    std::string currentLine;
    while ( std::getline( fileStream, currentLine ) )
    {
        if ( currentLine.empty() )
            continue;

        const char firstChar = currentLine[ 0 ];
        if ( isIniTokenComment( firstChar ) )
        {
            continue;
        }
        else if ( isIniTokenSectionBracketOpen( firstChar ) )
        {
            // This enables us to grab another section header
            if ( mStatus == IniStatus::READY_TO_PARSE_KEY_VALUE )
            {
                mStatus = IniStatus::READY_TO_PARSE_SECTION;
            }

            if ( mStatus == IniStatus::READY_TO_PARSE_SECTION )
            {
                parseSection( currentLine );
                mStatus = IniStatus::READY_TO_PARSE_KEY_VALUE;
            }
        }
        else
        {
            if ( mStatus == IniStatus::READY_TO_PARSE_KEY_VALUE )
            {
                parseKeyValue( currentLine );
            }
        }
    }

    // Save Ini File Data Entry
    Core::ParserDataRegistry::instance()->setParserData( mParserID, std::any( IniData{ mData } ) );

    return;
}

void Core::IniParser::parseSection( const std::string& currentLine )
{
    mCurrentSection.clear();

    // Index starts at 1 because of the '[' at the beginning
    for ( long unsigned int i = 1; i < currentLine.size(); i++ )
    {
        if ( !( isIniTokenSectionBracketEnd( currentLine[ i ] ) ) )
        {
            mCurrentSection += currentLine[ i ];
        }
        else
        {
            break;
        }
    }

    // Insert Section Header
    mData[ mCurrentSection ] = KeyValueData{};

    return;
}

void Core::IniParser::parseKeyValue( const std::string& currentLine )
{
    std::string key;
    std::string value;

    size_t position = currentLine.find( Core::IniToken::KEY_VALUE_ASSIGNMENT );
    if ( position == std::string::npos )
        return;

    key = currentLine.substr( 0, position );
    value = currentLine.substr( position + 1, currentLine.size() );

    // Insert into IniData
    auto it = mData.find( mCurrentSection );
    if ( it != mData.end() )
    {
        it->second.insert( { key, value } );
    }

    return;
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