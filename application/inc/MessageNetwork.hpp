#pragma once

#include "Message.hpp"
#include "MessageNodeInfo.hpp"

#include <functional>
#include <string>
#include <queue>
#include <map>
#include <memory>
#include <set>

class MessageNetwork
{
    public:
        MessageNetwork();

        void sendMessage(Message* message);

        void addSubscriber(const MessageNodeInfo& subscriber);
        void notifySubscribers();

    private:
        std::multimap< Message::ID, MessageNodeInfo > mSubscriberList;
        std::queue< std::unique_ptr< Message > > mMessageQueue;
};