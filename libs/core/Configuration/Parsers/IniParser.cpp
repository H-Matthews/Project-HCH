#include "core/Configuration/Parsers/IniParser.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/TextFileSink.hpp"

#include "utility/StringOperations.hpp"

Core::IniParser::IniParser( const std::string parserIdentifierString ) :
    Parser( parserIdentifierString, Parsers::ID::INI ),
    mStatus( Core::IniParser::IniStatus::READY_TO_PARSE_SECTION ),
    mCurrentActiveSection(),
    mLogger( nullptr )
{
    initializeLogger();
}

// TODO: Get Fine Level Logging for Parsing of files
bool Core::IniParser::parseFile( std::ifstream& fileStream, const std::string& fileName )
{
    std::string logMessage;
    if ( !fileStream.is_open() )
    {
        if constexpr ( Utility::CAN_LOG )
        {
            logMessage = "FileStream for file '" + fileName + "' was NOT OPEN. This file will NOT be parsed";
            mLogger->logError( logMessage );
        }
        return false;
    }

    // Parser is used to parse multiple files, so we must ensure the state is
    // clear when beginning to parse a file
    mCurrentActiveSection.clear();
    mStatus = IniStatus::READY_TO_PARSE_SECTION;

    // Container for parsed file contents
    IniData iniDataStructure;

    std::string currentLine;
    while ( std::getline( fileStream, currentLine ) )
    {
        // IF, its an empty line, then go next
        if ( currentLine.empty() )
            continue;

        // Each line should ignore whitespace in INI files
        currentLine = Utility::trimTrailingAndLeadingWhiteSpace( currentLine );

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
                auto assignmentPosition = currentLine.find( Core::IniToken::KEY_VALUE_ASSIGNMENT );
                if ( assignmentPosition == std::string::npos )
                    continue;

                auto keyValuePair = parseKeyValue( currentLine, assignmentPosition );

                // Ensure the values are not empty
                if ( keyValuePair.first == "" && keyValuePair.second == "" )
                    continue;

                insertKeyValue( keyValuePair, iniDataStructure );
            }
        }
    }

    if constexpr ( Utility::CAN_LOG )
    {
        logMessage = "Parsed File: '" + fileName + "'";
        mLogger->logInfo( logMessage );
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

    currentSection = Utility::removeAllSpaces( currentSection );

    // Set this var for convenience when inserting key values
    mCurrentActiveSection = currentSection;

    return std::make_pair( currentSection, isSubSection );
}

std::pair< std::string, std::string > Core::IniParser::parseKeyValue( const std::string& currentLine, size_t position )
{
    std::string key;
    std::string value;

    // Two operations, 1. gets substring 2. trims whitespace
    key = Utility::removeAllSpaces( currentLine.substr( 0, position ) );

    value = currentLine.substr( position + 1, currentLine.size() );

    size_t quotePositionIT = value.find( Core::IniToken::QUOTE );
    if ( quotePositionIT == std::string::npos )
    {
        value = Utility::removeAllSpaces( currentLine.substr( position + 1, currentLine.size() ) );
    }
    else
    {
        value = Utility::removeQuotes( value );
    }

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

bool Core::IniParser::isIniTokenComment( const char token ) const
{
    for ( const char& iniComment : Core::IniToken::COMMENT )
    {
        if ( token == iniComment )
            return true;
    }

    return false;
}

bool Core::IniParser::isIniTokenSectionBracketOpen( const char token ) const
{
    if ( token == Core::IniToken::SECTION_BRACKET_OPEN )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenSectionBracketEnd( const char token ) const
{
    if ( token == Core::IniToken::SECTION_BRACKET_END )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenKeyValueAssignment( const char token ) const
{
    if ( token == Core::IniToken::KEY_VALUE_ASSIGNMENT )
        return true;

    return false;
}

bool Core::IniParser::isIniTokenSubSection( const char token ) const
{
    if ( token == Core::IniToken::SUB_SECTION )
        return true;

    return false;
}

void Core::IniParser::initializeLogger()
{
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();
    if ( outDirectory == "" )
        return;

    mLogger = Utility::createTextFileLogger( "IniParser", outDirectory, "IniParser", ".log", Utility::LogLevel::DEBUG );

    if constexpr ( Utility::CAN_LOG )
    {
        mLogger->logInfo( "Logger Initialized" );
        Utility::LogRegistry::instance()->getGlobalLogger()->logInfo( "Logger Initialized" );
    }

    return;
}