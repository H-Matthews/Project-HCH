#include <gtest/gtest.h>

// Unit to be Tested
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

// Test Fixture
class UtilityLoggerTest : public ::testing::Test 
{
    protected:
        std::stringstream mBuffer;
        std::streambuf* sBuf;

        // Define Loggers
        std::shared_ptr< Utility::Logger > logger_default;
        std::shared_ptr< Utility::Logger > logger_oneSink;
        std::shared_ptr< Utility::Logger > logger_twoSinks;

        // Define Sinks
        std::shared_ptr< Utility::TextFileSink > textFileSink;
        std::shared_ptr< Utility::ColorConsoleSink > colorConsoleSink;

        std::vector< Utility::LogSinksI* > sinks;
        
        void SetUp() override 
        {
            logger_default = std::make_shared< Utility::Logger >("logger_default");
            logger_oneSink = std::make_shared< Utility::Logger >("logger_oneSink");
            logger_twoSinks = std::make_shared< Utility::Logger >("logger_twoSinks");

            // Create and add sink to logger_default
            colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
            logger_oneSink->addSink(colorConsoleSink);

            // Add Two sinks to logger_twoSinks
            textFileSink = std::make_shared< Utility::TextFileSink >("./testing_output", "Utility_logger_test", ".log");
            logger_twoSinks->addSink(colorConsoleSink);
            logger_twoSinks->addSink(textFileSink);

            // Hold couts buffer reference so we can restore later
            sBuf = std::cout.rdbuf();

            // Reroute couts buffer to ours
            std::cout.rdbuf(mBuffer.rdbuf());
        }

        void TearDown() override 
        {
            std::cout.rdbuf(sBuf);

            // Not memory leak, since they point to shared_pointers
            sinks.clear();
        }
};

/* 
    Default Constuctor Test
    Expected Member Values
    1. LoggerName --> "logger_default"
    2. GlobalLogLevel --> LogLevel::NONE
    3. Sinks --> None
*/
TEST_F(UtilityLoggerTest, defaultConstructor) 
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // LoggerName
    EXPECT_EQ(logger_default->getLoggerName(), "logger_default");

    // GlobalLogLevel
    EXPECT_EQ(logger_default->getGlobalLogLevel(), Utility::LogLevel::NONE);

    // Ensure Sinks size is 0
    sinks = logger_default->getSinkReferences();
    ASSERT_EQ(sinks.size(), 0);
}

/*
    Creating Logger with One ConsoleSink
    Expected Member Values
    1. LoggerName --> "logger_oneSink"
    2. GlobalLogLevel --> LogLevel::NONE
    3. Sinks --> 1: ColorConsoleSink
*/
TEST_F(UtilityLoggerTest, loggerOneSink)
{
    // Ensure NOT NULL
    ASSERT_NE(logger_oneSink, nullptr);

    // Ensure the loggerName is correct
    EXPECT_EQ(logger_oneSink->getLoggerName(), "logger_oneSink");

    // Ensure Log Level is Correct
    EXPECT_EQ(logger_oneSink->getGlobalLogLevel(), Utility::LogLevel::NONE);

    // Ensure Sink Size is 1
    sinks = logger_oneSink->getSinkReferences();
    ASSERT_EQ(sinks.size(), 1);

    // Ensure the Sink is a ColorConsoleSink
    auto colorConsoleSink = dynamic_cast< Utility::ColorConsoleSink* >( sinks[0] );
    EXPECT_NE(colorConsoleSink, nullptr);
}

/*
    Creating Logger with Two Sinks
    Expected Member Values
    1. LoggerName --> "logger_twoSinks"
    2. GlobalLogLevel --> LogLevel::NONE
    3. Sinks --> 2: { ColorConsoleSink, TextFileSink }
*/
TEST_F(UtilityLoggerTest, loggerTwoSinks)
{
    // Ensure NOT NULL
    ASSERT_NE(logger_twoSinks, nullptr);

    // Ensure the loggerName is correct
    EXPECT_EQ(logger_twoSinks->getLoggerName(), "logger_twoSinks");

    // Ensure Log Level is Correct
    EXPECT_EQ(logger_twoSinks->getGlobalLogLevel(), Utility::LogLevel::NONE);

    // Ensure Sink Size is 2
    sinks = logger_twoSinks->getSinkReferences();
    ASSERT_EQ(sinks.size(), 2);

    // Ensure first Sink is a ColorConsoleSink
    auto colorConsoleSink = dynamic_cast< Utility::ColorConsoleSink* >( sinks[0] );
    EXPECT_NE(colorConsoleSink, nullptr);

    // Ensure second Sink is a textFileSink
    auto textFileSink = dynamic_cast< Utility::TextFileSink* >( sinks[1] );
    EXPECT_NE(textFileSink, nullptr);
}

/*
    Test: setLoggerName Name: "Application" via const std::string
    Description: Ensure the name is being set properly
    Expectation: getLoggerName should return "Application"
*/
TEST_F(UtilityLoggerTest, setLoggerNameBasicString)
{
    const std::string loggerName = "Application";

    // Ensure Logger Name
    logger_default->setLoggerName(loggerName);
    EXPECT_EQ(loggerName, logger_default->getLoggerName());
}

/*
    Test: setLoggerName Name: "Application" via const char*
    Description: Ensure the name is being set properly
    Expectation: getLoggerName should return "Application"
*/
TEST_F(UtilityLoggerTest, setLoggerNameConstChar)
{
    const char* loggerName = "Application";

    // Ensure Logger Name
    logger_default->setLoggerName(loggerName);
    EXPECT_EQ(loggerName, logger_default->getLoggerName());
}

/*
    Test: setLoggerName Name: "123456789" 
    Description: Ensure the name is being set properly
    Expectation: getLoggerName should return "123456789"
*/
TEST_F(UtilityLoggerTest, setLoggerNameJustNumbers)
{
    const std::string loggerName = "123456789";

    // Ensure Logger Name
    logger_default->setLoggerName(loggerName);
    EXPECT_EQ(loggerName, logger_default->getLoggerName());
}

/*
    Test: addSinkList sinkList = {colorConsoleSink, textFileSink}
    Description: Ensure the function addSinkList properly adds the sinks defined in the list to the Logger
    Expectation: logger_default should have two sinks colorConsoleSink, textFileSink
*/
TEST_F(UtilityLoggerTest, addSinkList)
{
    Utility::Logger::sinkList list = { textFileSink, colorConsoleSink };
    logger_default->addSinkList(list);

    // Ensure there are two sinks for logger_default
    sinks = logger_default->getSinkReferences();
    ASSERT_EQ(sinks.size(), 2);

    // Ensure the correct sinks are in logger_default
    auto textFileSink2 = dynamic_cast< Utility::TextFileSink* >(sinks[0]);
    ASSERT_NE(textFileSink2, nullptr);

    auto colorConsoleSink2 = dynamic_cast< Utility::ColorConsoleSink* >(sinks[1]);
    ASSERT_NE(colorConsoleSink2, nullptr);
}

// BEGIN LOGLEVEL TESTING

/*
    Test: shouldLog GlobalLogLevel: DEBUG, SinkLogLevel: NONE
    Description: Tests the loggers Global LogLevel when SinkLevel is at NONE
    Expectation: Should Log everything 
*/
TEST_F(UtilityLoggerTest, globalLogLevelDebug)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to Debug
    logger_default->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_TRUE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::NONE);
    EXPECT_TRUE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: INFO, SinkLogLevel: NONE
    Description: Tests the loggers Global LogLevel when SinkLevel is at NONE
    Expectation: Should Log everything except DEBUG
*/
TEST_F(UtilityLoggerTest, globalLogLevelInfo)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to Debug
    logger_default->setGlobalLogLevel(Utility::LogLevel::INFO);

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::NONE);
    EXPECT_TRUE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: WARN, SinkLogLevel: NONE
    Description: Tests the loggers Global LogLevel when SinkLevel is at NONE
    Expectation: Should Log everything except DEBUG, INFO 
*/
TEST_F(UtilityLoggerTest, globalLogLevelWarn)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to WARN
    logger_default->setGlobalLogLevel(Utility::LogLevel::WARN);

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::NONE);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: ERROR, SinkLogLevel: NONE
    Description: Tests the loggers Global LogLevel when SinkLevel is at NONE
    Expectation: Should Log everything except DEBUG, INFO, WARN
*/
TEST_F(UtilityLoggerTest, globalLogLevelError)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to ERROR
    logger_default->setGlobalLogLevel(Utility::LogLevel::ERROR);

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::NONE);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_FALSE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: NONE
    Description: Tests the loggers Global LogLevel when SinkLevel is at NONE
    Expectation: Should Log nothing
*/
TEST_F(UtilityLoggerTest, globalLogLevelNone)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, Utility::LogLevel::NONE);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, Utility::LogLevel::NONE);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, Utility::LogLevel::NONE);
    EXPECT_FALSE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, Utility::LogLevel::NONE);
    EXPECT_FALSE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: DEBUG
    Description: Tests the sinks LogLevel when GlobalLogLevel is at NONE 
    Expectation: Should log everything for the individual sink
*/
TEST_F(UtilityLoggerTest, sinkLogLevelDebug)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Dummy Sink LogLevel
    Utility::LogLevel dummySinkLevel = Utility::LogLevel::DEBUG;

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, dummySinkLevel);
    EXPECT_TRUE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, dummySinkLevel);
    EXPECT_TRUE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, dummySinkLevel);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, dummySinkLevel);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: INFO
    Description: Tests the sinks LogLevel when GlobalLogLevel is at NONE 
    Expectation: Should log everything except DEBUG for the individual sink
*/
TEST_F(UtilityLoggerTest, sinkLogLevelInfo)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Dummy Sink LogLevel
    Utility::LogLevel dummySinkLevel = Utility::LogLevel::INFO;

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, dummySinkLevel);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, dummySinkLevel);
    EXPECT_TRUE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, dummySinkLevel);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, dummySinkLevel);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: WARN
    Description: Tests the sinks LogLevel when GlobalLogLevel is at NONE 
    Expectation: Should log everything except DEBUG, INFO for the individual sink
*/
TEST_F(UtilityLoggerTest, sinkLogLevelWarn)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Dummy Sink LogLevel
    Utility::LogLevel dummySinkLevel = Utility::LogLevel::WARN;

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, dummySinkLevel);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, dummySinkLevel);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, dummySinkLevel);
    EXPECT_TRUE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, dummySinkLevel);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: ERROR
    Description: Tests the sinks LogLevel when GlobalLogLevel is at NONE 
    Expectation: Should log everything except DEBUG, INFO, WARN for the individual sink
*/
TEST_F(UtilityLoggerTest, sinkLogLevelError)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Dummy Sink LogLevel
    Utility::LogLevel dummySinkLevel = Utility::LogLevel::ERROR;

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, dummySinkLevel);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, dummySinkLevel);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, dummySinkLevel);
    EXPECT_FALSE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, dummySinkLevel);
    EXPECT_TRUE(logError);
}

/*
    Test: shouldLog GlobalLogLevel: NONE, SinkLogLevel: NONE
    Description: Tests the sinks LogLevel when GlobalLogLevel is at NONE 
    Expectation: Should log nothing
*/
TEST_F(UtilityLoggerTest, sinkLogLevelNone)
{
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_default, nullptr);

    // Set Global level to NONE
    logger_default->setGlobalLogLevel(Utility::LogLevel::NONE);

    // Dummy Sink LogLevel
    Utility::LogLevel dummySinkLevel = Utility::LogLevel::NONE;

    bool logDebug = logger_default->shouldLog(Utility::LogLevel::DEBUG, dummySinkLevel);
    EXPECT_FALSE(logDebug);

    bool logInfo = logger_default->shouldLog(Utility::LogLevel::INFO, dummySinkLevel);
    EXPECT_FALSE(logInfo);

    bool logWarn = logger_default->shouldLog(Utility::LogLevel::WARN, dummySinkLevel);
    EXPECT_FALSE(logWarn);

    bool logError = logger_default->shouldLog(Utility::LogLevel::ERROR, dummySinkLevel);
    EXPECT_FALSE(logError);
}

// END LOGLEVEL TESTING

/*
    Test: LogDebug Sink: ColorConsoleLogger Message: "This is a Test"
    Description: Tests for the correct output
    Expectation: Buffer should contain "This is a test", "[DEBUG]"
*/
TEST_F(UtilityLoggerTest, logDebug_ConsoleLogger_Test)
{   
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_oneSink, nullptr);

    // GlobalLogLevel --> DEBUG
    logger_oneSink->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    // Expect message to contain these substrings
    logger_oneSink->logDebug("This is a test");
    EXPECT_NE(mBuffer.str().find("[DEBUG]"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("This is a test"), std::string::npos);
}

/*
    Test: logInfo Sink: ColorConsoleLogger Message: "This is a Test"
    Description: Tests for the correct output
    Expectation: Buffer should contain "This is a test", "[INFO]"
*/
TEST_F(UtilityLoggerTest, logInfo_ConsoleLogger_Test)
{   
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_oneSink, nullptr);

    // GlobalLogLevel --> DEBUG
    logger_oneSink->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    // Expect message to contain these substrings
    logger_oneSink->logInfo("This is a test");
    EXPECT_NE(mBuffer.str().find("[INFO]"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("This is a test"), std::string::npos);
}

/*
    Test: logWarn Sink: ColorConsoleLogger Message: "This is a Test"
    Description: Tests for the correct output
    Expectation: Buffer should contain "This is a test", "[WARN]"
*/
TEST_F(UtilityLoggerTest, logWarn_ConsoleLogger_Test)
{   
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_oneSink, nullptr);

    // GlobalLogLevel --> DEBUG
    logger_oneSink->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    // Expect message to contain these substrings
    logger_oneSink->logWarn("This is a test");
    EXPECT_NE(mBuffer.str().find("[WARN]"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("This is a test"), std::string::npos);
}

/*
    Test: logError Sink: ColorConsoleLogger Message: "This is a Test"
    Description: Tests for the correct output
    Expectation: Buffer should contain "This is a test", "[ERROR]"
*/
TEST_F(UtilityLoggerTest, logError_ConsoleLogger_Test)
{   
    // Ensure logger is NOT NULL
    ASSERT_NE(logger_oneSink, nullptr);

    // GlobalLogLevel --> DEBUG
    logger_oneSink->setGlobalLogLevel(Utility::LogLevel::DEBUG);

    // Expect message to contain these substrings
    logger_oneSink->logError("This is a test");
    EXPECT_NE(mBuffer.str().find("[ERROR]"), std::string::npos);
    EXPECT_NE(mBuffer.str().find("This is a test"), std::string::npos);
}