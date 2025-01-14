#include "application/inc/PlayerInput.hpp"


PlayerInput::PlayerInput(Core::MessageNetwork* network) :
    Core::MessageNode(network),
    playerInputMessage(nullptr)
{
    MessageNode::mMessageNodeInfo.nodeName = "PlayerInput";

    // Initialize Message
    playerInputMessage = std::make_shared<PlayerInputMessage>(Messages::ID::PlayerActionMessage);
}

void PlayerInput::update()
{
    playerInputMessage->action = Action::MOVE_UP;

    if(playerInputMessage->action != Action::NONE)
        MessageNode::send(playerInputMessage);
}