#include <gtest/gtest.h>

// Unit to be Tested
#include "utility/inc/Logger.hpp"

class UtilityLoggerTest : public testing::Test 
{
  protected:

  // Define Instance we want to use with our Tests
  Utility::Logger logger_1;
};

// This is really useless, just wanted to see how fixtures work
TEST_F(UtilityLoggerTest, correctMessage) 
{
  EXPECT_EQ(logger_1.log("HELLO"), "HELLO");
}