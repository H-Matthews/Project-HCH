#include <gtest/gtest.h>

#include <filesystem>

// Unit(s) to be Tested
#include "core/inc/Configuration/Parsers/IniParser.hpp"

// BEGIN TESTING

const std::string PROJ_DIRECTORY = PROJECT_DIR;

// Path to Input Test Files
const std::string INPUT_FILE_DIRECTORY = PROJ_DIRECTORY + "/" + "tests/input_files/ini_parser";

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

TEST_F( CoreIniParserTest, SimpleFileParse )
{
    const std::string fileName = "SimpleSectionParse.ini";

    std::ifstream fileStream;
    setupFile( fileName, fileStream );

    // Retrieve data
    std::any genericData = Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, fileName );
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

TEST_F( CoreIniParserTest, badFileStream )
{
    std::ifstream fileStream;
    ASSERT_FALSE( mTestParser.parseFile( fileStream, "DummyFileName" ) );
}

TEST_F( CoreIniParserTest, BadSyntax )
{
    const std::string fileName = "BadSyntax.ini";

    std::ifstream fileStream;
    setupFile( fileName, fileStream );

    // Retrieve data
    std::any genericData = Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, fileName );
    ASSERT_TRUE( genericData.has_value() );

    ASSERT_NO_THROW( std::any_cast< Core::IniData >( genericData ) );
    Core::IniData iniData = std::any_cast< Core::IniData >( genericData );

    // Check Section Size
    EXPECT_TRUE( iniData.size() == 3 );

    // Check Section Test
    auto sectionTestIT = iniData.find( "Test" );
    ASSERT_FALSE( sectionTestIT == iniData.end() );

    // Check KeyValueSize
    auto keyValueIT = sectionTestIT->second->keyValues.find( "key" );
    EXPECT_FALSE( keyValueIT == sectionTestIT->second->keyValues.end() );
}

TEST_F( CoreIniParserTest, PracticalFileParse )
{
    const std::string fileName = "PracticalFileParse.ini";

    std::ifstream fileStream;
    setupFile( fileName, fileStream );

    // Retrieve data
    std::any genericData = Core::ParserDataRegistry::instance()->getParserDataStructure( Parsers::ID::INI, fileName );
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

// END TESTING