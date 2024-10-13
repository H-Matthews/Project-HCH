#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(const Message::ID messageID ) :
    Message( messageID, "PlayerActionMessage"),
    action()
{
}