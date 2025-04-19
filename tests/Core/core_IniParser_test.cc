#include <gtest/gtest.h>

#include <filesystem>

// Unit(s) to be Tested
#include "core/inc/Configuration/Parsers/IniParser.hpp"

// BEGIN TESTING

const std::string PROJ_DIRECTORY = PROJECT_DIR;

// Path to Input Test Files
const std::string INPUT_FILE_DIRECTORY = PROJ_DIRECTORY + "/" + "tests/input_files/ini_parser";

// Test Files
const std::string badSyntaxFile = "BadSyntax.ini";
const std::string simpleSectionParseFile = "SimpleSectionParse.ini";
const std::string practicalFileParseFile = "PracticalFileParse.ini";

// Test Fixture
class CoreIniParserTest : public ::testing::Test
{
  protected:
    Core::IniParser mTestParser;

  public:
    CoreIniParserTest() :
        mTestParser( "TestParser" )
    {}

    void SetUp() override
    {}

  protected:
    void setupFile( const std::string& fileName, std::ifstream& fileStream )
    {
        fileStream.open( INPUT_FILE_DIRECTORY + "/" + fileName, std::ios::in );
        ASSERT_TRUE( fileStream.is_open() );
        ASSERT_TRUE( mTestParser.parseFile( fileStream, fileName ) );
    }
};

/*
    Test: Basic File Parse
    Expected Values:
        Number of Sections: 1
        Section: App
            - KeyValues: 1
            - SubSections: 0
*/
TEST_F( CoreIniParserTest, SimpleFileParse )
{
    std::ifstream fileStream;
    setupFile( simpleSectionParseFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, simpleSectionParseFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Check Section Size
    EXPECT_TRUE( iniData.size() == 1 );

    // Check Section Data
    auto it = iniData.find( "SimpleSection" );
    ASSERT_FALSE( it == iniData.end() );
    EXPECT_EQ( it->second->sectionName, "SimpleSection" );

    // Check KeyValue Size
    EXPECT_TRUE( it->second->keyValues.size() == 1 );

    // Check KeyValue Data
    auto mapIT = it->second->keyValues.find( "key" );
    EXPECT_EQ( mapIT->first, "key" );
    EXPECT_EQ( mapIT->second, "value" );
}

/*
    Test: Practical File Parse
    Expected Values:
        Number Of Sections: 4
        Section: App
            KeyValues: 1
            SubSections: 1

        Section: App.Logger
            KeyValues: 2
            SubSections: 2

        Section: App.Logger.ColorConsoleSink
            KeyValues: 1
            SubSections: 0

        Section: App.Logger.TextFileSink
            KeyValues: 3
            SubSections: 0
*/
TEST_F( CoreIniParserTest, PracticalFileParse )
{
    std::ifstream fileStream;
    setupFile( practicalFileParseFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, practicalFileParseFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Check Section Size
    EXPECT_TRUE( iniData.size() == 4 );

    // Check Section App
    auto sectionIT = iniData.find( "App" );
    ASSERT_FALSE( sectionIT == iniData.end() );

    // Check App KeyValueSize
    EXPECT_TRUE( sectionIT->second->keyValues.size() == 1 );

    // Check App KeyValue (1)
    auto outDirIT = sectionIT->second->keyValues.find( "OutputDirectory" );
    ASSERT_FALSE( outDirIT == sectionIT->second->keyValues.end() );
    EXPECT_EQ( outDirIT->first, "OutputDirectory" );
    EXPECT_EQ( outDirIT->second, "output" );

    // Check "App" subsections
    auto subSectionVector = sectionIT->second->subSections;
    ASSERT_TRUE( subSectionVector.size() == 1 );

    // Grab the subsection "App.Logger"
    std::shared_ptr< Core::IniSection > subSectionLogger = subSectionVector.at( 0 );
    EXPECT_TRUE( subSectionLogger->keyValues.size() == 2 );

    // Check App.Logger KeyValue (1)
    auto loggerNameKeyIT = subSectionLogger->keyValues.find( "LoggerName" );
    ASSERT_FALSE( loggerNameKeyIT == subSectionLogger->keyValues.end() );
    EXPECT_EQ( loggerNameKeyIT->first, "LoggerName" );
    EXPECT_EQ( loggerNameKeyIT->second, "AppLogger" );

    // Check App.Logger KeyValue (2)
    auto GlobalLogLevelIT = subSectionLogger->keyValues.find( "GlobalLogLevel" );
    ASSERT_FALSE( GlobalLogLevelIT == subSectionLogger->keyValues.end() );
    EXPECT_EQ( GlobalLogLevelIT->first, "GlobalLogLevel" );
    EXPECT_EQ( GlobalLogLevelIT->second, "INFO" );

    // Check SubSections
    auto appLoggerSubsectionVector = subSectionLogger->subSections;
    EXPECT_TRUE( appLoggerSubsectionVector.size() == 2 );

    // Check ColorConsoleSink SubSection
    std::shared_ptr< Core::IniSection > appLoggerConsoleSection = appLoggerSubsectionVector.at( 0 );
    auto ConsoleSinkLogLevelIT = appLoggerConsoleSection->keyValues.find( "LogLevel" );
    ASSERT_FALSE( ConsoleSinkLogLevelIT == appLoggerConsoleSection->keyValues.end() );
    EXPECT_EQ( ConsoleSinkLogLevelIT->first, "LogLevel" );
    EXPECT_EQ( ConsoleSinkLogLevelIT->second, "INFO" );

    // Check TextFileSink SubSection
    std::shared_ptr< Core::IniSection > appLoggerTextSinkSection = appLoggerSubsectionVector.at( 1 );

    // Check KeyValue (1)
    auto appLoggerSinkLogFileIT = appLoggerTextSinkSection->keyValues.find( "LogFileName" );
    ASSERT_FALSE( appLoggerSinkLogFileIT == appLoggerTextSinkSection->keyValues.end() );
    EXPECT_EQ( appLoggerSinkLogFileIT->first, "LogFileName" );
    EXPECT_EQ( appLoggerSinkLogFileIT->second, "App" );

    // Check KeyValue (2)
    auto appLoggerFileExtIT = appLoggerTextSinkSection->keyValues.find( "LogFileExtension" );
    ASSERT_FALSE( appLoggerFileExtIT == appLoggerTextSinkSection->keyValues.end() );
    EXPECT_EQ( appLoggerFileExtIT->first, "LogFileExtension" );
    EXPECT_EQ( appLoggerFileExtIT->second, ".log" );

    // Check KeyValue (3)
    auto appLoggerLevelIT = appLoggerTextSinkSection->keyValues.find( "LogLevel" );
    ASSERT_FALSE( appLoggerLevelIT == appLoggerTextSinkSection->keyValues.end() );
    EXPECT_EQ( appLoggerLevelIT->first, "LogLevel" );
    EXPECT_EQ( appLoggerLevelIT->second, "ERROR" );
}

/*
    Test: Bad File Stream
*/
TEST_F( CoreIniParserTest, badFileStream )
{
    std::ifstream fileStream;
    ASSERT_FALSE( mTestParser.parseFile( fileStream, "DummyFileName" ) );
}

/*
    Test: Bad Syntax: Successful Parse
*/
TEST_F( CoreIniParserTest, BadSyntaxSuccessfullParse )
{
    std::ifstream fileStream;
    setupFile( badSyntaxFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, badSyntaxFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
}

/*
    Test: Bad Syntax: Has correct amount of sections
    Expected Values:
        Number of Sections: 3
*/
TEST_F( CoreIniParserTest, BadSyntaxCorrectSectionSize )
{
    std::ifstream fileStream;
    setupFile( badSyntaxFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, badSyntaxFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Check Section Size
    EXPECT_TRUE( iniData.size() == 3 );
}

/*
    Test: Bad Syntax: Section contains leading spaces
    Expected Values:
        Section: Hello
            KeyValues: 2
            SubSections: 0
*/
TEST_F( CoreIniParserTest, BadSyntaxLeadingSpaceSection )
{
    std::ifstream fileStream;
    setupFile( badSyntaxFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, badSyntaxFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Check that the section was parsed
    auto sectionHelloIT = iniData.find( "Hello" );
    ASSERT_FALSE( sectionHelloIT == iniData.end() );

    // Check Key Values
    auto helloKeyValues = sectionHelloIT->second->keyValues;
    EXPECT_TRUE( helloKeyValues.size() == 2 );

    // Check KeyValues (1)
    auto helloKeyValuesOne = helloKeyValues.find( "key" );
    EXPECT_EQ( helloKeyValuesOne->first, "key" );
    EXPECT_EQ( helloKeyValuesOne->second, "myvalue" );

    // Check KeyValues (2)
    auto helloKeyValuesTwo = helloKeyValues.find( "key2" );
    EXPECT_EQ( helloKeyValuesTwo->first, "key2" );
    EXPECT_EQ( helloKeyValuesTwo->second, "my value" );
}

/*
    Test: Bad Syntax: KeyValue has leading and trailing whitespace
    Expected Values:
        Section: Test
            KeyValues: 1
            SubSections: 0
*/
TEST_F( CoreIniParserTest, BadSyntaxLeadingAndTrailingSpaceKeyValues )
{
    std::ifstream fileStream;
    setupFile( badSyntaxFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, badSyntaxFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Grab Section Test
    auto sectionTestIT = iniData.find( "Test" );
    ASSERT_FALSE( sectionTestIT == iniData.end() );

    // Check KeyValues
    auto testKeyValues = sectionTestIT->second->keyValues;
    EXPECT_TRUE( testKeyValues.size() == 1 );

    auto testKeyValueIT = testKeyValues.find( "key" );
    ASSERT_FALSE( testKeyValueIT == testKeyValues.end() );

    EXPECT_EQ( testKeyValueIT->first, "key" );
    EXPECT_EQ( testKeyValueIT->second, "value" );
}

/*
    Test: Bad Syntax: Section has spaces in it
    Expected Values:
        Section: TESTSPACESINSECTION
            KeyValues: 1
            SubSections: 0
*/
TEST_F( CoreIniParserTest, BadSyntaxSpacesInSection )
{
    std::ifstream fileStream;
    setupFile( badSyntaxFile, fileStream );

    // Retrieve data
    std::any genericData =
        Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, badSyntaxFile );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Grab Section
    auto spacesInSection = iniData.find( "TESTSPACESINSECTION" );
    ASSERT_FALSE( spacesInSection == iniData.end() );

    // Check Key Values
    auto spacesInSectionKeyValues = spacesInSection->second->keyValues;
    EXPECT_TRUE( spacesInSectionKeyValues.size() == 1 );
}

// END TESTING