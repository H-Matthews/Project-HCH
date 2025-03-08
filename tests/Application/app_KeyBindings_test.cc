#include <gtest/gtest.h>

// Unit(s) to be Tested
#include "application/inc/KeyBindings.hpp"

// Ensure the Constructor is setting keys to the correct values
TEST( App_KeyBindings_Constructor_Test, defaultConstructor )
{
    Application::KeyBindings keyBindings;
    Actions::Player action( Actions::Player::NONE );

    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::A );
    EXPECT_EQ( action, Actions::Player::MOVE_LEFT );

    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::D );
    EXPECT_EQ( action, Actions::Player::MOVE_RIGHT );

    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::W );
    EXPECT_EQ( action, Actions::Player::MOVE_UP );

    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::S );
    EXPECT_EQ( action, Actions::Player::MOVE_DOWN );

    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::Space );
    EXPECT_EQ( action, Actions::Player::JUMP );
}

TEST( App_KeyBindings_AssignKey_Test, assignKey )
{
    Application::KeyBindings keyBindings;
    Actions::Player action( Actions::Player::NONE );
    keyBindings.assignKey( Actions::Player::MOVE_UP, sf::Keyboard::Scan::T );

    // Ensure the W Key is unassigned (Returns PlayerAction::NONE)
    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::W );
    EXPECT_EQ( action, Actions::Player::NONE );

    // Ensure the T Key was assigned to PlayerAction:MOVE_UP
    action = keyBindings.getAssignedAction( sf::Keyboard::Scan::T );
    EXPECT_EQ( action, Actions::Player::MOVE_UP );
}