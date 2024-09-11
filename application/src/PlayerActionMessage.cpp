#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(const std::pair< Message::ID, std::string >& messageID ) :
    Message( messageID ),
    action()
{
}

PlayerActionMessage::PlayerActionMessage( PlayerActionMessage* message)
{
    mMessageInfo.sender = message->mMessageInfo.sender;
    mMessageInfo.messageID = message->mMessageInfo.messageID;
    action = message->action;
}