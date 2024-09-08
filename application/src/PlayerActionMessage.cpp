#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage(MessageNetwork& messageNetwork, std::string identifierString) :
    Message(messageNetwork, identifierString)
{
}

PlayerActionMessage::PlayerActionMessage()
{
    mTopicList.reserve(MESSAGE_TYPE_SIZE);
}

PlayerActionMessage::PlayerActionMessage( PlayerActionMessage* message)
{
    mTopicList = message->mTopicList;
    mSender = message->mSender;
    mIdentifierString = message->mIdentifierString;
}


void PlayerActionMessage::buildMessage(std::vector< Messages::ID > messageTopics, std::string sender)
{
    mTopicList = messageTopics;
    mSender = sender;
}
