#include "application/inc/Messages/PlayerInputMessage.hpp"


Application::PlayerInputMessage::PlayerInputMessage(const Messages::ID messageID) :
    Message(messageID, "PlayerActionMessage"),
    action(PlayerAction::NONE)
{
}