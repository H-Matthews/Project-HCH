#pragma once

#include "core/inc/Messaging/Message.hpp"
#include "core/inc/Messaging/MessageNode.hpp"
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

            void notifySubscribers();

            void initializeLogger();
            void shutdownNetwork();
        private:
            void publishMessage(std::shared_ptr<Message> message);

            void addPublisherTopic(const std::string& nodeName, Messages::ID messageID);
            void addSubscriberTopic(const std::string& nodeName, Messages::ID messageID);

            void registerSubscriberNode(const std::string& nodeName, std::function<void (Message*) > callback);
            void registerPublisherNode(const std::string& nodeName);

            void removeTopicFromPublisher(const std::string& nodeName, Messages::ID messageID);
            void removeTopicFromSubscriber(const std::string& nodeName, Messages::ID messageID);

            void addressPendingRequests();

        private:
            std::queue< std::shared_ptr< Message > > mMessageQueue;

            std::map< std::size_t, std::set< Messages::ID > > mSubscriberNodes;
            std::map< std::size_t, std::set< Messages::ID > > mPublisherNodes;

            std::map< std::size_t, std::function<void (Message*) > > mSubscriberCallBacks;
            
            std::map< std::size_t, std::set< Messages::ID > > mPendingPublisherRequests;
            std::map< std::size_t, std::set< Messages::ID > > mPendingSubscriberRequests;

            std::hash< std::string > mHash;
            std::shared_ptr< Utility::Logger > mLogger;

        public:
            friend class Core::MessageNode;
    };

}