#pragma once

#include "core/inc/Messaging/MessageNodeInfo.hpp"

#include <functional>

namespace Core
{
    class MessageNetwork;

    class MessageNode 
    {
        public:
            MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);
            MessageNode(MessageNetwork* messageNetwork);

        protected:
            void subscribeTo(Messages::ID subscribeMessageID);
            void notifyUnsubscribe(Messages::ID messageID);
            void registerSubscriberMessages();
            void send(Message* message);
            virtual void onNotify(Message* message);

        private:
            std::function<void (Message*)> getNotifyFunc();

        private:
            MessageNetwork* mMessageNetwork;

        protected:
            MessageNodeInfo mMessageNodeInfo;
    };

}
