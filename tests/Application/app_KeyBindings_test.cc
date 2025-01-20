#include <gtest/gtest.h>

// Unit(s) to be Tested
#include "application/inc/KeyBindings.hpp"

// Ensure the Constructor is setting keys to the correct values
TEST(App_KeyBindings_Constructor_Test, defaultConstructor)
{
    KeyBindings keyBindings;
    PlayerAction action(PlayerAction::NONE);

    action = keyBindings.getAssignedAction(sf::Keyboard::A);
    EXPECT_EQ(action, PlayerAction::MOVE_LEFT);

    action = keyBindings.getAssignedAction(sf::Keyboard::D);
    EXPECT_EQ(action, PlayerAction::MOVE_RIGHT);

    action = keyBindings.getAssignedAction(sf::Keyboard::W);
    EXPECT_EQ(action, PlayerAction::MOVE_UP);

    action = keyBindings.getAssignedAction(sf::Keyboard::S);
    EXPECT_EQ(action, PlayerAction::MOVE_DOWN);

    action = keyBindings.getAssignedAction(sf::Keyboard::Space);
    EXPECT_EQ(action, PlayerAction::JUMP);
}

TEST(App_KeyBindings_AssignKey_Test, assignKey)
{
    KeyBindings keyBindings;
    PlayerAction action(PlayerAction::NONE);
    keyBindings.assignKey(PlayerAction::MOVE_UP, sf::Keyboard::T);

    // Ensure the W Key is unassigned (Returns PlayerAction::NONE)
    action = keyBindings.getAssignedAction(sf::Keyboard::W);
    EXPECT_EQ(action, PlayerAction::NONE);

    // Ensure the T Key was assigned to PlayerAction:MOVE_UP
    action = keyBindings.getAssignedAction(sf::Keyboard::T);
    EXPECT_EQ(action, PlayerAction::MOVE_UP);

}