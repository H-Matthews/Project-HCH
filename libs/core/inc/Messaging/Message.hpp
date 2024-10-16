#pragma once

#include <string>
#include <memory>

class MessageNetwork;

class Message
{
    public:

        enum class ID
        {
            NONE = 0,
            PlayerActionMessage,
            EnemySpawnMessage
        };

        struct Info 
        {
            Message::ID messageID;
            std::string stringMessageID;
            std::string sender;

            Info();
            Info(Message::ID messageID, const std::string& stringMessageID);
        };

    public:
        virtual ~Message();
        virtual std::unique_ptr<Message> clone() const;

        void setSender(const std::string& sender);

        std::string getSenderName() const;
        Message::ID getMessageID() const;
        const std::string& getStringMessageID() const;
    
    protected:
        Message(const Message::ID messageID, const std::string& stringMessageID );

    protected:
        Info mMessageInfo;
};  