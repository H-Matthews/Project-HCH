#pragma once

#include "core/inc/Messaging/Message.hpp"
#include "core/inc/Messaging/MessageNode.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <queue>
#include <map>
#include <set>

namespace Core
{

    struct SubscriberNodeInfo
    {
        std::string mNodename;
        std::function<void (Message*) > mCallback;

        SubscriberNodeInfo(const std::string& nodeName, std::function<void (Message*) > func) : 
            mNodename(nodeName),
            mCallback(func) {}
    };

    struct PublisherNodeInfo
    {
        std::string mNodeName;

        PublisherNodeInfo(const std::string& nodeName) : 
            mNodeName(nodeName) {}
    };

    /**
     * MessageNetwork defines a basic Pub / Sub system. This class contains the data structures that operate
     * as the "network"
     * NOTE: Not Thread safe
     */
    class MessageNetwork
    {
        public:
            MessageNetwork();

            void notifySubscribers();

            void initializeLogger();
            void shutdownNetwork();
        private:

            void registerSubscriberNode(const std::string& nodeName, std::function<void (Message*) > callback);
            void registerPublisherNode(const std::string& nodeName);

            void addSubscriberTopic(const std::string& nodeName, Messages::ID messageID);
            void addPublisherTopic(const std::string& nodeName, Messages::ID messageID);

            void unRegisterSubscriberNode(const std::string& nodeName);
            void unRegisterPublisherNode(const std::string& nodeName);

            // These functions do not Immediately remove the topics from each Node. It puts the request into the
            // data structures mPendingPublisherRequests & mPendingSubscriberRequests
            // The function addressPendingRequests does the actual removal
            void removeTopicFromPublisher(const std::string& nodeName, Messages::ID messageID);
            void removeTopicFromSubscriber(const std::string& nodeName, Messages::ID messageID);

            void publishMessage(std::shared_ptr<Message> message);

            void addressPendingRequests();

            void shutdownNetwork();
        private:
            std::queue< std::shared_ptr< Message > > mMessageQueue;

            std::map< std::size_t, std::set< Messages::ID > > mSubscriberNodes;
            std::map< std::size_t, std::set< Messages::ID > > mPublisherNodes;

            // Used for storing information about the Publisher / Subscriber Node
            std::map< std::size_t, SubscriberNodeInfo > mSubscriberRecords;
            std::map< std::size_t, PublisherNodeInfo > mPublisherRecords;
            
            std::map< std::size_t, std::set< Messages::ID > > mPendingPublisherRequests;
            std::map< std::size_t, std::set< Messages::ID > > mPendingSubscriberRequests;

            std::hash< std::string > mHash;
            std::shared_ptr< Utility::Logger > mLogger;

        public:
            friend class Core::MessageNode;
    };

}