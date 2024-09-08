#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(MessageNetwork& messageNetwork, std::string identifierString) :
    Message(messageNetwork, identifierString)
{
}

PlayerActionMessage::PlayerActionMessage()
{
}

PlayerActionMessage::PlayerActionMessage( PlayerActionMessage* message)
{
    mSender = message->mSender;
    mIdentifierString = message->mIdentifierString;
    mIDType = message->mIDType;
}
