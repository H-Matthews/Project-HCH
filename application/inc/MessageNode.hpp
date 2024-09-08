#pragma once

#include "MessageNetwork.hpp"

#include <string>
#include <array>

class MessageNetwork;

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);

        std::unique_ptr<Message> createMessage(Messages::ID messageID);

    protected:
        void registerSubscriberTopics();

        std::function<void (Message*)> getNotifyFunc();
        void send(Message* message);
        virtual void onNotify(Message* message);

    protected:
        MessageNetwork* mMessageNetwork;

        MessageSubscriptionInfo mSubscriptionInfo;
        std::string mPublisherName;

        std::vector< Messages::ID > mSubscribeToTopics;
        std::vector< Messages::ID > mPublishToTopics;
};