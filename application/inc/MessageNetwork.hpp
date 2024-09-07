#pragma once
#include "Message.hpp"

#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

struct MessageSubscriptionInfo
{
    std::string name;
    std::function<void(Message*)> callback;

    MessageSubscriptionInfo(std::string nodeName) { name = nodeName; }
};

class MessageNetwork
{
    public:
        MessageNetwork();

        void sendMessage(Message* message);

        void addSubscriber(std::pair< std::vector< MessageTopic >, MessageSubscriptionInfo > subscriber);
        void notifySubscribers();

    private:
        std::multimap< MessageTopic, MessageSubscriptionInfo > mSubscriberList;
        std::queue< std::unique_ptr< Message > > mMessageQueue;
};