#include "Message.hpp"

Message::Info::Info()
{
    sender = "";
    messageID.first = Message::ID::NONE;
    messageID.second = "NONE";
}

Message::Info::Info(const std::pair<ID, std::string >& messageID) :
    messageID(messageID),
    sender("")
{
}

Message::Message()
{
}

Message::Message(const std::pair<ID, std::string >& messageID) :
    mMessageInfo(messageID)
{
}

Message::~Message()
{
}

void Message::setSender(const std::string& sender)
{
    mMessageInfo.sender = sender;
}

Message::ID Message::getMessageID() const
{
    return mMessageInfo.messageID.first;
}

std::string Message::getSenderName() const
{
    return mMessageInfo.sender;
}

std::string Message::getStringMessageID() const
{
    return mMessageInfo.messageID.second;
}