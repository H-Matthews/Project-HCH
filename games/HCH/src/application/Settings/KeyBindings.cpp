#include "application/Settings/KeyBindings.hpp"

Application::KeyBindings::KeyBindings() {
    // Set Initial Keys
    mKeyBindings[Core::Key::A] = Actions::Player::MOVE_LEFT;
    mKeyBindings[Core::Key::D] = Actions::Player::MOVE_RIGHT;
    mKeyBindings[Core::Key::S] = Actions::Player::MOVE_DOWN;
    mKeyBindings[Core::Key::W] = Actions::Player::MOVE_UP;
    mKeyBindings[Core::Key::Left] = Actions::Player::MOVE_LEFT;
    mKeyBindings[Core::Key::Right] = Actions::Player::MOVE_RIGHT;
    mKeyBindings[Core::Key::Down] = Actions::Player::MOVE_DOWN;
    mKeyBindings[Core::Key::Up] = Actions::Player::MOVE_UP;
    mKeyBindings[Core::Key::Space] = Actions::Player::ATTACK;
    mKeyBindings[Core::Key::J] = Actions::Player::ATTACK;
}

void Application::KeyBindings::assignKey(Actions::Player action, Core::Key key) {
    // Remove the action from other keybindings if necessary
    for (auto iterator = mKeyBindings.begin(); iterator != mKeyBindings.end();) {
        if (iterator->second == action) {
            mKeyBindings.erase(iterator++);
        } else {
            ++iterator;
        }
    }

    // Insert new Binding
    mKeyBindings[key] = action;
}

Actions::Player Application::KeyBindings::getAssignedAction(Core::Key key) const {
    Actions::Player action = Actions::Player::NONE;
    auto found = mKeyBindings.find(key);

    if (found != mKeyBindings.end()) {
        action = found->second;
    }

    return action;
}

const std::map<Core::Key, Actions::Player>& Application::KeyBindings::getKeyBindings() {
    return mKeyBindings;
}

// Real time actions are typically gameplay related actions
// such as movement, or attacking. They will require
// the key to be pressed down at that moment
bool Application::KeyBindings::isRealTimeAction(Actions::Player action) {
    bool isRealTimeAction = false;

    switch (action) {
    case Actions::Player::MOVE_DOWN:
    case Actions::Player::MOVE_UP:
    case Actions::Player::MOVE_LEFT:
    case Actions::Player::MOVE_RIGHT:
        isRealTimeAction = true;
        break;
    default:
        isRealTimeAction = false;
    }

    return isRealTimeAction;
}