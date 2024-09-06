#include "MessageTopicFlag.hpp"


MessageTopicFlag::MessageTopicFlag() :
mFlagValue(0)
{
}

void MessageTopicFlag::setFlag(MessageTopic flag)
{
    mFlagValue |= (int)flag;
}

void MessageTopicFlag::unsetFlag(MessageTopic flag)
{
    mFlagValue &= ~(int)flag;
}

void MessageTopicFlag::resetFlag()
{
    mFlagValue = 0;
}

bool MessageTopicFlag::hasFlag(MessageTopic flag)
{
    return (mFlagValue & (int)flag) == (int)flag;
}

uint8_t MessageTopicFlag::getFlagValue()
{
    return mFlagValue;
}

std::vector< MessageTopic > MessageTopicFlag::determineTopics()
{
    std::vector< MessageTopic > messageTopics;
    if(hasFlag(MessageTopic::PLAYER))
        messageTopics.push_back(MessageTopic::PLAYER);

    if(hasFlag(MessageTopic::ENEMY))
        messageTopics.push_back(MessageTopic::ENEMY);

    if(hasFlag(MessageTopic::DROPS))
        messageTopics.push_back(MessageTopic::DROPS);

    return messageTopics;
}

