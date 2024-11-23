#include <gtest/gtest.h>

// Unit to be Tested
#include "utility/inc/StringOperations.hpp"

TEST(Utility_StringOperations_toString_Test, primitives)
{
  // Case: Integer
  EXPECT_EQ(Utility::toString<int>(10), "10");
  EXPECT_NE(Utility::toString<int>(10), "10 ");

  // Case: Float
  EXPECT_EQ(Utility::toString<float>(10.55), "10.55");
  EXPECT_NE(Utility::toString<float>(10.55), "10.55 ");

  // Case: Double
  EXPECT_EQ(Utility::toString<double>(10.5), "10.5");
  EXPECT_NE(Utility::toString<double>(10.5), "10.5 ");

  // Case: Char
  EXPECT_EQ(Utility::toString<char>('a'), "a");
  EXPECT_NE(Utility::toString<char>('a'), "a ");
}