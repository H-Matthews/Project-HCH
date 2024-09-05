#include "Message.hpp"

Message::Message(MessageTopicFlag topicFlag) :
mTopicFlag(topicFlag)
{
    if(mTopicFlag.getFlagValue() != 0)
        mTopicList = mTopicFlag.determineTopics();
}

void Message::determineTopics()
{
    mTopicList = mTopicFlag.determineTopics();
}

std::vector< MessageTopic > Message::getTopicList()
{
    return mTopicList;
}