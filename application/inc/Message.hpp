#pragma once

#include <string>
#include <set>

constexpr uint8_t MESSAGE_TYPE_SIZE = 3;

class MessageNetwork;

namespace Messages 
{
    enum class ID
    {
        None = 0,
        PlayerActionMessage,
        EnemySpawnMessage
    };
}

class Message
{
    public:

    public:
        Message();
        Message(MessageNetwork& messageNetwork, std::string identifierString);
        Message(Message* message);
        virtual ~Message();

        void populateMessageHeader(Messages::ID IDType, std::string sender);

        std::string getSenderName() const;
        Messages::ID getMessageID() const;

    protected:
        MessageNetwork* mMessageNetwork;
        Messages::ID mIDType;
        std::string mIdentifierString;
        std::string mSender;
};  