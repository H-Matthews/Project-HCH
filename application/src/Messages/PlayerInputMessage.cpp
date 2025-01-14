#include "application/inc/Messages/PlayerInputMessage.hpp"


PlayerInputMessage::PlayerInputMessage(const Messages::ID messageID) :
    Message(messageID, "PlayerActionMessage"),
    action(Action::NONE)
{
}