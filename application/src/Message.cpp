#include "Message.hpp"

Message::Message() :
    mMessageNetwork(),
    mIDType(),
    mIdentifierString(),
    mSender()
{
}

Message::Message(MessageNetwork& messageNetwork, std::string identifierString) :
    mMessageNetwork(&messageNetwork),
    mIDType(),
    mIdentifierString(identifierString),
    mSender()
{
}

void Message::populateMessageHeader( Messages::ID IDType, std::string sender)
{
    mIDType = IDType;
    mSender = sender;
}

Message::~Message()
{
}

const std::string Message::getSenderName() const
{
    return mSender;
}

Messages::ID Message::getMessageID() const
{
    return mIDType;
}