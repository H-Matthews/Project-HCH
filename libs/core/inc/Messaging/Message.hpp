#pragma once

#include "core/inc/Messaging/MessageTypes.hpp"

#include <string>
#include <memory>

namespace Core
{
    class MessageNetwork;

    /**
     * Message is the Base class for all Messaging types that go onto the MessageNetwork
     */
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
            virtual Message* clone() const = 0;

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