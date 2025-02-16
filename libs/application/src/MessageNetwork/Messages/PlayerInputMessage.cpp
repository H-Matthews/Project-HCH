#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"


Application::PlayerInputMessage::PlayerInputMessage(const Messages::ID messageID) :
    Message(messageID, "PlayerActionMessage"),
    action(Actions::Player::NONE)
{
}