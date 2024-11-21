#include <gtest/gtest.h>

// Unit to be Tested
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

// Test Fixture
class UtilityLoggerTest : public ::testing::Test 
{
    protected:
        std::stringstream mBuffer;
        std::streambuf* sBuf;
        std::shared_ptr< Utility::Logger > logger_default;

        std::vector< Utility::LogSinksI* > sinks;
        
        void SetUp() override 
        {
            logger_default = std::make_shared< Utility::Logger >("logger_default");

            // Create and add sink to logger_default
            auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
            logger_default->addSink(colorConsoleSink);

            sinks = logger_default->getSinkReferences();

            // Hold couts buffer reference so we can restore later
            sBuf = std::cout.rdbuf();

            // Reroute couts buffer to ours
            std::cout.rdbuf(mBuffer.rdbuf());
        }

        void TearDown() override 
        {
            std::cout.rdbuf(sBuf);

            // Not memory leak, since we are getting references 
            sinks.clear();
        }
};

/* 
    Default Constuctor Test
    Expected Member Values
    1. LoggerName --> "" 
    2. GlobalLogLevel --> LogLevel::INFO
    3. Sinks --> ColorConsoleSink
*/
TEST_F(UtilityLoggerTest, defaultConstructor) 
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // LoggerName
    EXPECT_EQ(logger_default->getLoggerName(), "logger_default");

    // GlobalLogLevel
    EXPECT_EQ(logger_default->getGlobalLogLevel(), Utility::LogLevel::NONE);

    // Ensure Sinks size is 1 and the object is a ColorConsoleSink
    ASSERT_EQ(sinks.size(), 1);
    auto consoleSink = dynamic_cast<Utility::ColorConsoleSink*>(sinks[0]);
    ASSERT_NE(consoleSink, nullptr);
}

/*
    shouldLog Function Test
*/
TEST_F(UtilityLoggerTest, shouldLog)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Sink LogLevel to DEBUG and make sure it logs ALL levels
    bool ret1 = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::DEBUG);
    EXPECT_TRUE(ret1);

    bool ret2 = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::DEBUG);
    EXPECT_TRUE(ret2);

    bool ret3 = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::DEBUG);
    EXPECT_TRUE(ret3);

    bool ret4 = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::DEBUG);
    EXPECT_TRUE(ret4);

    bool ret5 = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::DEBUG);
    EXPECT_TRUE(ret5);

    // Set Sink LogLevel to INFO and make sure it DOES NOT LOG DEBUG
    bool ret6 = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::INFO);
    EXPECT_FALSE(ret6);

    // Test globalLogLevel; Default Constructor sets it at NONE
    bool ret7 = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(ret7);

    bool ret8 = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_FALSE(ret8);

    bool ret9 = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_FALSE(ret9);

    // Set GlobalLogLevel & sinkLogLevel to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);
    bool ret10 = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(ret10);
}

TEST_F(UtilityLoggerTest, validOutputSanityCheck)
{   
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // GlobalLogLevel --> DEBUG
    logger_default->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    // Expect message to contain these substrings
    logger_default->logDebug("This is a test");
    EXPECT_NE(mBuffer.str().find("[DEBUG]"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("This is a test"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("utility_Logger_test.cc"), std::string::npos);
}

TEST_F(UtilityLoggerTest, sinkLoggingCheck)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // GlobalLogLevel --> NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Ensure sinks size is 1
    ASSERT_EQ(sinks.size(), 1);

    sinks[0]->setSinkLogLevel(Utility::LogLevel::INFO);

    logger_default->logDebug("This is a test");

    // Expect Buffer to be empty
    ASSERT_EQ(mBuffer.str(), "");

    logger_default->logInfo("This is a test");
    ASSERT_NE(mBuffer.str(), "");
}