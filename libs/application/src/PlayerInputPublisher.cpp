#include "application/inc/PlayerInputPublisher.hpp"

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
    PlayerAction action = mKeyBindings.getAssignedAction(keyPressedEvent.scancode);

    if(action == PlayerAction::NONE)
    {
        return;
    }

    mPlayerActionMessage->action = mKeyBindings.getAssignedAction(keyPressedEvent.scancode);
    MessageNode::send(mPlayerActionMessage);
}