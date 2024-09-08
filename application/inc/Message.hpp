#pragma once

#include <vector>
#include <string>

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

        virtual void buildMessage(std::vector< Messages::ID > messageTopics, std::string sender) = 0;

        std::vector< Messages::ID > getTopicList();
        std::string getSenderName();

    protected:
        MessageNetwork* mMessageNetwork;
        std::string mIdentifierString;

        std::vector< Messages::ID > mTopicList;
        std::string mSender;
};  