#pragma once

#include <string>
#include <memory>

namespace Core
{
    class MessageNetwork;

    namespace Messages
    {
        enum class ID
        {
            NONE = 0,
            PlayerActionMessage,
            EnemySpawnMessage
        };
    }

    class Message
    {
        public:

            struct Info 
            {
                Messages::ID messageID;
                std::string stringMessageID;
                std::string sender;

                Info();
                Info(Messages::ID messageID, const std::string& stringMessageID);
            };

        public:
            virtual ~Message();
            virtual std::unique_ptr<Message> clone() const;

            void setSender(const std::string& sender);

            std::string getSenderName() const;
            Messages::ID getMessageID() const;
            const std::string& getStringMessageID() const;
        
        protected:
            Message(const Messages::ID messageID, const std::string& stringMessageID );

        protected:
            Info mMessageInfo;
    };  

}