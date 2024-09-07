#pragma once

#include "MessageNetwork.hpp"

#include <string>
#include <array>

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);

    protected:
        void registerSubscriberTopics();

        std::function<void (Message*)> getNotifyFunc();
        void send(Message* message);
        virtual void onNotify(Message* message);

    protected:
        MessageNetwork* mMessageNetwork;

        MessageSubscriptionInfo mSubscriptionInfo;
        std::string mPublisherName;

        std::vector< MessageTopic > mSubscribeToTopics;
        std::vector< MessageTopic > mPublishToTopics;
};