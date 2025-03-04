#pragma once

#include "core/inc/Messaging/Message.hpp"
#include "core/inc/Messaging/MessageNodeInfo.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

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

            void sendMessage(std::shared_ptr<Message> message);

            void addSubscriber(const MessageNodeInfo& subscriber);
            bool isDuplicateSubscriber(const Messages::ID key, const std::string& nodeName);

            void insertUnsubscriber(const Messages::ID& mesasgeID, const std::string& nodeName);
            void notifySubscribers();

            void initializeLogger();

            void shutdownNetwork();
        private:
            void unSubscribe();

        private:
            std::shared_ptr< Utility::Logger > mLogger;

            std::multimap< Messages::ID, MessageNodeInfo > mSubscriberList;
            std::multimap< Messages::ID, std::string > mUnsubscribeList;
            std::queue< std::shared_ptr< Message > > mMessageQueue;
    };

}