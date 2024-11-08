#include <gtest/gtest.h>

// Unit to be Tested
#include "utility/inc/StringOperations.hpp"

// Demonstrate some basic assertions.
// These are Trivial on purpose
TEST(Utility_StringOperations_StatesEnumToStr_Test, confirmInput) 
{
  // Case: None
  EXPECT_EQ(Utility::statesEnumToString(States::ID::NONE), "None");
  EXPECT_NE(Utility::statesEnumToString(States::ID::NONE), "NONE");

  // Case: Menu
  EXPECT_EQ(Utility::statesEnumToString(States::ID::Menu), "Menu State");
  EXPECT_NE(Utility::statesEnumToString(States::ID::Menu), "MENU STATE");

  // Case: Game
  EXPECT_EQ(Utility::statesEnumToString(States::ID::Game), "Game State");
  EXPECT_NE(Utility::statesEnumToString(States::ID::Game), "GAME STATE");

  // Case: Pause
  EXPECT_EQ(Utility::statesEnumToString(States::ID::Pause), "Pause State");
  EXPECT_NE(Utility::statesEnumToString(States::ID::Pause), "PAUSE STATE");
}

TEST(Utility_StringOperations_MessageEnumToStr_Test, confirmInput)
{
  // Case: None
  EXPECT_EQ(Utility::messageIDEnumToString(Messages::ID::NONE), "None");
  EXPECT_NE(Utility::messageIDEnumToString(Messages::ID::NONE), "NONE");

  // Case: PlayerActionMessage
  EXPECT_EQ(Utility::messageIDEnumToString(Messages::ID::PlayerActionMessage), "PlayerAction Message");
  EXPECT_NE(Utility::messageIDEnumToString(Messages::ID::PlayerActionMessage), "PLAYERACTION MESSAGE");

  // Case: EnemySpawnMessage
  EXPECT_EQ(Utility::messageIDEnumToString(Messages::ID::EnemySpawnMessage), "EnemySpawn Message");
  EXPECT_NE(Utility::messageIDEnumToString(Messages::ID::EnemySpawnMessage), "ENEMYSPAWN MESSAGE");
}


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