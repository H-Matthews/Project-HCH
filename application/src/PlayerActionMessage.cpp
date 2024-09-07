#include "PlayerActionMessage.hpp"

PlayerActionMessage::PlayerActionMessage()
{
    mTopicList.reserve(MESSAGE_TOPIC_SIZE);
}

PlayerActionMessage::PlayerActionMessage( PlayerActionMessage* message)
{
    mTopicList = message->mTopicList;
    mSender = message->mSender;
}

void PlayerActionMessage::buildMessage(std::vector< MessageTopic > messageTopics, std::string sender)
{
    mTopicList = messageTopics;
    mSender = sender;
}
