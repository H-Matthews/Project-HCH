#include "core/inc/Messaging/Message.hpp"

Core::Message::Info::Info()
{
    sender = "";
    messageID = Messages::ID::NONE;
}

Core::Message::Info::Info(const Messages::ID messageID, const std::string& stringMessageID) :
    messageID(messageID),
    stringMessageID(stringMessageID),
    sender("")
{
}
Core::Message::Message(const Messages::ID messageID, const std::string& stringMessageID) :
    mMessageInfo(messageID, stringMessageID)
{
}

Core::Message::~Message()
{
}

std::unique_ptr<Core::Message> Core::Message::clone() const
{
    return std::make_unique<Message>(*this);
}

void Core::Message::setSender(const std::string& sender)
{
    mMessageInfo.sender = sender;
}

std::string Core::Message::getSenderName() const
{
    return mMessageInfo.sender;
}

Core::Messages::ID Core::Message::getMessageID() const
{
    return mMessageInfo.messageID;
}

const std::string& Core::Message::getStringMessageID() const
{
    return mMessageInfo.stringMessageID;
}