#include "application/inc/MessageNetwork/Publishers/PlayerInputPublisher.hpp"

#include <iostream>

Application::PlayerInputPublisher::PlayerInputPublisher(Core::MessageNetwork* network, Application::KeyBindings keyBindings) :
    Core::MessageNode(network),
    mKeyBindings(keyBindings)
{
    MessageNode::mMessageNodeInfo.nodeName = "PlayerInput Publisher";

    // Initialize Messages
    mPlayerActionMessage = std::make_shared<Application::PlayerInputMessage>(Messages::ID::PlayerActionMessage);
}


void Application::PlayerInputPublisher::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent)
{
    Actions::Player playerAction = mKeyBindings.getAssignedAction(keyPressedEvent.scancode);

    if(playerAction == Actions::Player::NONE)
        return;

    mPlayerActionMessage->action = playerAction;
    MessageNode::send(mPlayerActionMessage);
}