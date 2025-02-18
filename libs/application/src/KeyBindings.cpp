#include "application/inc/KeyBindings.hpp"

Application::KeyBindings::KeyBindings()
{
    // Set Initial Keys
    mKeyBindings[sf::Keyboard::Scan::A] = Actions::Player::MOVE_LEFT;
    mKeyBindings[sf::Keyboard::Scan::D] = Actions::Player::MOVE_RIGHT;
    mKeyBindings[sf::Keyboard::Scan::S] = Actions::Player::MOVE_DOWN;
    mKeyBindings[sf::Keyboard::Scan::W] = Actions::Player::MOVE_UP;
    mKeyBindings[sf::Keyboard::Scan::Space] = Actions::Player::JUMP;
}

void Application::KeyBindings::assignKey(Actions::Player action, sf::Keyboard::Scan scanCode)
{
    // Remove the action from other keybindings if necessary
    for(auto iterator = mKeyBindings.begin(); iterator != mKeyBindings.end(); )
    {
        if(iterator->second == action)
            mKeyBindings.erase(iterator++);
        else
            ++iterator;
    }

    // Insert new Binding
    mKeyBindings[scanCode] = action;
}

Actions::Player Application::KeyBindings::getAssignedAction(sf::Keyboard::Scan scanCode) const
{
    Actions::Player action = Actions::Player::NONE;
    auto found = mKeyBindings.find(scanCode);

    if(found != mKeyBindings.end())
        action = found->second;

    return action;
}

const std::map<sf::Keyboard::Scan, Actions::Player>& Application::KeyBindings::getKeyBindings()
{
    return mKeyBindings;
}

// Real time actions are typically gameplay related actions
// such as movement, or attacking. They will require 
// the key to be pressed down at that moment
bool Application::KeyBindings::isRealTimeAction(Actions::Player action)
{
    bool isRealTimeAction = false;

    switch(action)
    {
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