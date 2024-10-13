#include "core/inc/Messaging/Message.hpp"

Message::Info::Info()
{
    sender = "";
    messageID = Message::ID::NONE;
}

Message::Info::Info(const Message::ID messageID, const std::string& stringMessageID) :
    messageID(messageID),
    stringMessageID(stringMessageID),
    sender("")
{
}
Message::Message(const Message::ID messageID, const std::string& stringMessageID) :
    mMessageInfo(messageID, stringMessageID)
{
}

Message::~Message()
{
}

std::unique_ptr<Message> Message::clone() const
{
    return std::make_unique<Message>(*this);
}

void Message::setSender(const std::string& sender)
{
    mMessageInfo.sender = sender;
}

std::string Message::getSenderName() const
{
    return mMessageInfo.sender;
}

Message::ID Message::getMessageID() const
{
    return mMessageInfo.messageID;
}

const std::string& Message::getStringMessageID() const
{
    return mMessageInfo.stringMessageID;
}