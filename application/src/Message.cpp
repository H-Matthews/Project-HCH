#include "Message.hpp"

Message::Message() :
    mIdentifierString()
{
}

Message::Message(MessageNetwork& messageNetwork, std::string identifierString) :
    mMessageNetwork(&messageNetwork),
    mIdentifierString(identifierString)
{
}

Message::Message(Message* message)
{
}

Message::~Message()
{
}

std::vector< Messages::ID > Message::getTopicList()
{
    return mTopicList;
}

std::string Message::getSenderName()
{
    return mSender;
}