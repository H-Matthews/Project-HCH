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
            PlayerActionMessage
        };

        struct Info 
        {
            ID messageID;
            std::string sender;

            Info() { messageID = Message::ID::NONE; sender = ""; }
            Info(const Message::ID messageID);
        };

    public:
        Message();
        virtual ~Message();
        Message::ID getMessageID() const;

        void setSender(const std::string& sender);
        const std::string& getSenderName() const;
    
    protected:
        Message(const Message::ID ID );

    protected:
        Info mMessageInfo;
};  