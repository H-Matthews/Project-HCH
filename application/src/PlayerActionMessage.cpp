#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(Message::ID ID ) :
    Message(ID),
    action()
{
}

PlayerActionMessage::PlayerActionMessage( PlayerActionMessage* message)
{
    mMessageInfo.sender = message->mMessageInfo.sender;
    mMessageInfo.messageID = message->mMessageInfo.messageID;
    action = message->action;
}