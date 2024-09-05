#pragma once

#include "MessageNetwork.hpp"

#include <string>

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);

    protected:
        std::function<void (Message)> getNotifyFunc();
        void send(Message message);
        virtual void onNotify(Message message);

        void registerSubscriberTopics();

    protected:
        MessageNetwork* mMessageNetwork;

        MessageTopicFlag mSubscribeToTopics;
        MessageTopicFlag mPublishToTopics;
        std::string mMessageNodeName;
};