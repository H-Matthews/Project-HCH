#pragma once

#include "core/inc/Messaging/Message.hpp"
#include "core/inc/Messaging/MessageNodeInfo.hpp"

#include <string>
#include <queue>
#include <map>
#include <memory>

namespace Core
{

    class MessageNetwork
    {
        public:
            MessageNetwork();

            void sendMessage(Message* message);

            void addSubscriber(const MessageNodeInfo& subscriber);
            void insertUnsubscriber(const Messages::ID& mesasgeID, const std::string& nodeName);
            void notifySubscribers();

        private:
            void unSubscribe();

        private:
            std::multimap< Messages::ID, MessageNodeInfo > mSubscriberList;
            std::multimap< Messages::ID, std::string > mUnsubscribeList;
            std::queue< std::unique_ptr< Message > > mMessageQueue;
    };

}