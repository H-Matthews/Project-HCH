#include "application/inc/MessageNetwork/Publishers/PlayerInputPublisher.hpp"

Application::PlayerInputPublisher::PlayerInputPublisher(Core::MessageNetwork& network, Application::KeyBindings keyBindings) :
    Core::MessageNode(network),
    mKeyBindings(keyBindings)
{
    MessageNode::mMessageNodeInfo.nodeName = "PlayerInput Publisher";

    // Initialize Messages
    mPlayerActionMessage = std::make_unique<Application::PlayerInputMessage>(Messages::ID::PlayerActionMessage);
}

/*
    Handles KeyPressed events. Checks the keyBindings to see IF the current key is mapped to a Player Action
    IF its mapped to an action AND that action is not listed as a realtimeAction, then we send the message
*/
void Application::PlayerInputPublisher::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent)
{
    Actions::Player playerAction = mKeyBindings.getAssignedAction(keyPressedEvent.scancode);

    if(playerAction == Actions::Player::NONE || mKeyBindings.isRealTimeAction(playerAction))
        return;

    PlayerInputMessage* clonedMessage = mPlayerActionMessage->clone();
    std::shared_ptr<PlayerInputMessage> message(clonedMessage);

    message->action = playerAction;
    MessageNode::send(message);
}

/*
    This function is called at least once every frame. This removes the 250ms Operating System delay that is caused
    when trying to hold down a key. There is no delay when getting key presses with this function, so it needs to be used 
    for gameplay level keys
*/
void Application::PlayerInputPublisher::handleRealTimeInput()
{
    for(const auto& pair : mKeyBindings.getKeyBindings())
    {
        if(sf::Keyboard::isKeyPressed(pair.first) && mKeyBindings.isRealTimeAction(pair.second))
        {
            // Have to clone here
            PlayerInputMessage* clonedMessage = mPlayerActionMessage->clone();
            std::shared_ptr<PlayerInputMessage> message(clonedMessage);

            message->action = pair.second;
            MessageNode::send(message);
        }
    }
}