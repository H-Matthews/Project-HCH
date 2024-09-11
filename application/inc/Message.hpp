#pragma once

#include <string>

constexpr uint8_t MESSAGE_TYPE_SIZE = 3;

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
            std::pair<ID, std::string> messageID;
            std::string sender;

            Info();
            Info(const std::pair<ID, std::string >& messageID);
        };

    public:
        Message();
        virtual ~Message();

        void setSender(const std::string& sender);

        std::string getSenderName() const;
        Message::ID getMessageID() const;
        std::string getStringMessageID() const;
    
    protected:
        Message(const std::pair<ID, std::string >& messageID );

    protected:
        Info mMessageInfo;
};  