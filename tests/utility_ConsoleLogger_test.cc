#include <gtest/gtest.h>

#include <limits>

// Unit to be Tested
#include "utility/inc/ConsoleLogger.hpp"

class UtilityConsoleLoggerTest : public ::testing::Test 
{
  protected:
    std::stringstream buffer;
    std::streambuf* sbuf;
    Utility::ConsoleLogger consoleLogger;

    void SetUp() override 
    {
      // Hold couts buffer reference here
      sbuf = std::cout.rdbuf();

      // Reroute couts buffer to OUR buffer
      std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override 
    {
      std::cout.rdbuf(sbuf);
    }
};

TEST_F(UtilityConsoleLoggerTest, LogsDebugMessage)
{
  consoleLogger.log("Debug Message", Utility::LogLevel::DEBUG, "utility_ConsoleLogger_test.cc", 1);

  // Expect messages to contain these substrings
  EXPECT_NE(buffer.str().find("[DEBUG]"), std::string::npos);
  EXPECT_NE(buffer.str().find("Debug Message"), std::string::npos);
  EXPECT_NE(buffer.str().find("utility_ConsoleLogger_test.cc:1"), std::string::npos);
}

TEST_F(UtilityConsoleLoggerTest, LogsInfoMessage)
{
  consoleLogger.log("Info Message", Utility::LogLevel::INFO, nullptr, 23);

  // Expect messages to contain these substrings
  EXPECT_NE(buffer.str().find("[INFO]"), std::string::npos);
  EXPECT_NE(buffer.str().find("Info Message"), std::string::npos);

  // Expect messages to NOT contain substring
  EXPECT_EQ(buffer.str().find("utility_ConsoleLogger_test.cc:23"), std::string::npos);
}

TEST_F(UtilityConsoleLoggerTest, LogsWarningMessage)
{
  consoleLogger.log("Warning Message", Utility::LogLevel::WARNING, "test.cpp", 69);

  // Expect messages to contain substrings
  EXPECT_NE(buffer.str().find("[WARNING]"), std::string::npos);
  EXPECT_NE(buffer.str().find("Warning Message"), std::string::npos);
  EXPECT_NE(buffer.str().find("test.cpp:69"), std::string::npos);
}

TEST_F(UtilityConsoleLoggerTest, LogsErrorMessage)
{
  consoleLogger.log("Error Message", Utility::LogLevel::ERROR, "test.cpp", 82);

  // Expect messages to contain substrings
  EXPECT_NE(buffer.str().find("[ERROR]"), std::string::npos);
  EXPECT_NE(buffer.str().find("Error Message"), std::string::npos);
  EXPECT_NE(buffer.str().find("test.cpp:82"), std::string::npos);
}

TEST_F(UtilityConsoleLoggerTest, LogsEmptyMessage)
{
  consoleLogger.log("", Utility::LogLevel::INFO, nullptr, 82);

  // Expect message to contain substring
  EXPECT_NE(buffer.str().find(""), std::string::npos);

  // Expect message to NOT contain substring
  EXPECT_EQ(buffer.str().find("82"), std::string::npos);
}