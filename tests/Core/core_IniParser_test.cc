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
    EXPECT_FALSE( it == iniData.end() );
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
    EXPECT_FALSE( sectionIT == iniData.end() );
}

// END TESTING