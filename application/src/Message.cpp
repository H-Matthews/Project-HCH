#include "Message.hpp"

Message::Info::Info(const Message::ID messageID) :
    messageID(messageID),
    sender("")
{
}

Message::Message()
{
}

Message::Message(const Message::ID ID) :
    mMessageInfo(ID)
{
}

Message::~Message()
{
}

Message::ID Message::getMessageID() const
{
    return mMessageInfo.messageID;
}

const std::string& Message::getSenderName() const
{
    return mMessageInfo.sender;
}

void Message::setSender(const std::string& sender)
{
    mMessageInfo.sender = sender;
}