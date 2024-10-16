#pragma once

#include "MessageNodeInfo.hpp"

#include <functional>

class MessageNetwork;

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName);
        MessageNode(MessageNetwork* messageNetwork);

    protected:
        void subscribeTo(Message::ID subscribeMessageID);
        void notifyUnsubscribe(Message::ID messageID);
        void registerSubscriberMessages();
        void send(Message* message);
        virtual void onNotify(Message* message);

    private:
        std::function<void (Message*)> getNotifyFunc();

    private:
        MessageNetwork* mMessageNetwork;

    protected:
        MessageNodeInfo mMessageNodeInfo;
};