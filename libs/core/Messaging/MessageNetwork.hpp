#pragma once

#include "core/Messaging/Message.hpp"
#include "core/Messaging/MessageNode.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include "utility/Logging/Logger.hpp"

#include <queue>
#include <map>
#include <memory>
#include <set>

namespace Core
{

    struct SubscriberNodeInfo
    {
        std::string mNodename;
        std::function<void(Message*)> mCallback;

        SubscriberNodeInfo(const std::string& nodeName, std::function<void(Message*)> func) :
            mNodename(nodeName),
            mCallback(func)
        {}
    };

    struct PublisherNodeInfo
    {
        std::string mNodeName;

        PublisherNodeInfo(const std::string& nodeName) :
            mNodeName(nodeName)
        {}
    };

    class MessageNetwork
    {
      public:
        static constexpr std::string_view SECTION_NAME = "MessageNetwork";

        explicit MessageNetwork(const ConfigSection* config = nullptr);

        void notifySubscribers();

        void shutdownNetwork();

      private:
        void registerSubscriberNode(const std::string& nodeName, std::function<void(Message*)> callback);
        void registerPublisherNode(const std::string& nodeName);

        void addSubscriberTopic(const std::string& nodeName, Messages::ID messageID);
        void addPublisherTopic(const std::string& nodeName, Messages::ID messageID);

        void unRegisterSubscriberNode(const std::string& nodeName);
        void unRegisterPublisherNode(const std::string& nodeName);

        void removeTopicFromPublisher(const std::string& nodeName, Messages::ID messageID);
        void removeTopicFromSubscriber(const std::string& nodeName, Messages::ID messageID);

        void publishMessage(std::shared_ptr<Message> message);

        void addressPendingRequests();

      private:
        std::queue<std::shared_ptr<Message>> mMessageQueue;

        std::map<std::size_t, std::set<Messages::ID>> mSubscriberNodes;
        std::map<std::size_t, std::set<Messages::ID>> mPublisherNodes;

        std::map<std::size_t, SubscriberNodeInfo> mSubscriberRecords;
        std::map<std::size_t, PublisherNodeInfo> mPublisherRecords;

        std::map<std::size_t, std::set<Messages::ID>> mPendingPublisherRequests;
        std::map<std::size_t, std::set<Messages::ID>> mPendingSubscriberRequests;

        std::hash<std::string> mHash;

        std::shared_ptr<Utility::Logger> mLogger;

      public:
        friend class Core::MessageNode;
    };

}
