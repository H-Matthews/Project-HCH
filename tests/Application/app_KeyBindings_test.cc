#include <gtest/gtest.h>

// Unit(s) to be Tested
#include "application/Settings/KeyBindings.hpp"

/*
    Test: Default Constructor
    Expected Values:
        A --> MOVE_LEFT
        D --> MOVE_RIGHT
        W --> MOVE_UP
        S --> MOVE_DOWN
        SPACE --> ATTACK
*/
TEST(App_KeyBindings_Constructor_Test, defaultConstructor) {
    Application::KeyBindings keyBindings;
    Actions::Player action(Actions::Player::NONE);

    action = keyBindings.getAssignedAction(Core::Key::A);
    EXPECT_EQ(action, Actions::Player::MOVE_LEFT);

    action = keyBindings.getAssignedAction(Core::Key::D);
    EXPECT_EQ(action, Actions::Player::MOVE_RIGHT);

    action = keyBindings.getAssignedAction(Core::Key::W);
    EXPECT_EQ(action, Actions::Player::MOVE_UP);

    action = keyBindings.getAssignedAction(Core::Key::S);
    EXPECT_EQ(action, Actions::Player::MOVE_DOWN);

    action = keyBindings.getAssignedAction(Core::Key::Space);
    EXPECT_EQ(action, Actions::Player::ATTACK);
}

/*
    Test: Assign Key function
    Expected Values:
        T --> MOVE_UP

        W --> NONE (W was previously assigned to MOVE_UP)
*/
TEST(App_KeyBindings_AssignKey_Test, assignKey) {
    Application::KeyBindings keyBindings;
    Actions::Player action(Actions::Player::NONE);
    keyBindings.assignKey(Actions::Player::MOVE_UP, Core::Key::T);

    // Ensure the W Key is unassigned (Returns PlayerAction::NONE)
    action = keyBindings.getAssignedAction(Core::Key::W);
    EXPECT_EQ(action, Actions::Player::NONE);

    // Ensure the T Key was assigned to PlayerAction:MOVE_UP
    action = keyBindings.getAssignedAction(Core::Key::T);
    EXPECT_EQ(action, Actions::Player::MOVE_UP);
}