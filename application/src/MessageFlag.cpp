#include "MessageFlag.hpp"


MessageFlag::MessageFlag(uint8_t flagValue) :
mFlagValue(flagValue)
{
}

void MessageFlag::setFlag(MessageTopics flag)
{
    mFlagValue |= (int)flag;
}

void MessageFlag::unsetFlag(MessageTopics flag)
{
    mFlagValue &= ~(int)flag;
}

bool MessageFlag::hasFlag(MessageTopics flag)
{
    return (mFlagValue & (int)flag) == (int)flag;
}

uint8_t MessageFlag::getFlagValue()
{
    return mFlagValue;
}

