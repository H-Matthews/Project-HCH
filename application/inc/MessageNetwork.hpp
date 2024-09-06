#pragma once
#include "Message.hpp"

#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <map>

struct MessageSubscriptionInfo
{
    std::string name;
    std::function<void(Message)> callback;

    MessageSubscriptionInfo(std::string nodeName) { name = nodeName; }
};

class MessageNetwork
{
    public:
        MessageNetwork();

        void sendMessage(Message message);

        void addSubscriber(std::pair< MessageTopicFlag, MessageSubscriptionInfo > subscriber);
        void notifySubscribers();

    private:
        std::multimap< MessageTopic, MessageSubscriptionInfo > mSubscriberList;
        std::queue<Message> mMessageQueue;
};