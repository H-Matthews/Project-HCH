#pragma once
#include "Message.hpp"

#include <functional>
#include <vector>
#include <queue>
#include <map>

class MessageNetwork
{
    public:
        MessageNetwork();
        void addSubscriber(MessageTopicFlag topicFlag, std::function<void (Message)> messageListener);
        void sendMessage(Message message);
        void notifySubscribers();

    private:
        bool registerTopic(MessageTopic topic);

        std::map< MessageTopic, std::vector<std::function<void (Message)>>> mSubscriberList;
        std::queue<Message> mMessageQueue;
};