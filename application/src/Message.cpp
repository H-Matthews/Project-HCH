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

Message::Message(Message*)
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

std::string Message::getSenderName() const
{
    return mSender;
}

Messages::ID Message::getMessageID() const
{
    return mIDType;
}