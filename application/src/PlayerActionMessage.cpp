#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(const Message::ID messageID ) :
    Message( messageID, "PlayerActionMessage"),
    action()
{
}

std::unique_ptr<Message> PlayerActionMessage::clone() const
{
    return std::make_unique<PlayerActionMessage>(*this);
}