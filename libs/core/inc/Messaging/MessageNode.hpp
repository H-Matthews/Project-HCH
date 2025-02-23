#pragma once

#include "core/inc/Messaging/MessageNodeInfo.hpp"
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/LogRegistry.hpp"

#include <functional>

namespace Core
{
    class MessageNetwork;

    enum class NodeType
    {
        SUBSCRIBER = 0,
        PUBLISHER,
        SUB_AND_PUB
    };

    class MessageNode 
    {
        public:
            MessageNode(MessageNetwork& messageNetwork, const std::string& messageNodeName, NodeType nodeType);

        protected:
            void addTopic(Messages::ID messageID);
            void addSubscriberTopic(Messages::ID messageID);
            void addPublisherTopic(Messages::ID messageID);

            void removeTopic(Messages::ID messageID);
            void removeSubscriberTopic(Messages::ID messageID);
            void removePublisherTopic(Messages::ID messageID);

            void publish(std::shared_ptr<Message> message);
            virtual void onNotify(Message* message);

        private:
            std::function<void (Message*)> getNotifyFunc();

        private:
            MessageNetwork& mMessageNetwork;

        protected:
            MessageNodeInfo mMessageNodeInfo;
            NodeType mNodeType;

            std::shared_ptr<Utility::Logger> mNetworkLogger;
    };

}
