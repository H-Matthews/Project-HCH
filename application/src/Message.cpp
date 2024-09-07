#include "Message.hpp"

Message::Message()
{
    mTopicList.reserve(MESSAGE_TOPIC_SIZE);
}

void Message::buildMessage(std::vector< MessageTopic > messageTopics)
{
    mTopicList = messageTopics;
}

std::vector< MessageTopic > Message::getTopicList()
{
    return mTopicList;
}