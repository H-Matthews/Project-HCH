#include "Message.hpp"

Message::Message() :
mTopicFlag()
{
}

void Message::setMessageTopicFlag(MessageTopicFlag topicFlag)
{
    mTopicFlag = topicFlag;
}

void Message::populateTopicList()
{
    mTopicList = mTopicFlag.determineTopics();
}

MessageTopicFlag Message::getTopicFlag()
{
    return mTopicFlag;
}

std::vector< MessageTopic > Message::getTopicList()
{
    return mTopicList;
}