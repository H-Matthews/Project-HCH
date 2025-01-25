#include "application/inc/KeyBindings.hpp"

Application::KeyBindings::KeyBindings()
{
    // Set Initial Keys
    mKeyBindings[sf::Keyboard::A] = PlayerAction::MOVE_LEFT;
    mKeyBindings[sf::Keyboard::D] = PlayerAction::MOVE_RIGHT;
    mKeyBindings[sf::Keyboard::S] = PlayerAction::MOVE_DOWN;
    mKeyBindings[sf::Keyboard::W] = PlayerAction::MOVE_UP;
    mKeyBindings[sf::Keyboard::Space] = PlayerAction::JUMP;
}

void Application::KeyBindings::assignKey(PlayerAction action, sf::Keyboard::Key key)
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
    mKeyBindings[key] = action;
}

Application::PlayerAction Application::KeyBindings::getAssignedAction(sf::Keyboard::Key key) const
{
    PlayerAction action = PlayerAction::NONE;
    auto found = mKeyBindings.find(key);

    if(found != mKeyBindings.end())
        action = found->second;

    return action;
}

// Real time actions are typically gameplay related actions
// such as movement, or attacking. They will require 
// the key to be pressed down at that moment
bool Application::KeyBindings::isRealTimeAction(PlayerAction action)
{
    bool isRealTimeAction = false;

    switch(action)
    {
        case PlayerAction::MOVE_DOWN:
        case PlayerAction::MOVE_UP:
        case PlayerAction::MOVE_LEFT:
        case PlayerAction::MOVE_RIGHT:
        case PlayerAction::JUMP:
            isRealTimeAction = true;
            break;
        default:
            isRealTimeAction = false;
    }

    return isRealTimeAction;
}