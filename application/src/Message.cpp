#include "Message.hpp"

Message::~Message()
{
}

std::vector< MessageTopic > Message::getTopicList()
{
    return mTopicList;
}

std::string Message::getSenderName()
{
    return mSender;
}