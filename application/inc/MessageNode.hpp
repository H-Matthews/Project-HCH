#pragma once

#include "MessageNetwork.hpp"

class MessageNode 
{
    public:
        MessageNode(MessageNetwork* messageNetwork);

    protected:
        std::function<void (Message)> getNotifyFunc();
        void send(Message message);
        virtual void onNotify(Message message);

        void registerMessageListener();

    protected:
        MessageNetwork* mMessageNetwork;
        MessageFlag mSubscribeTopics;
        MessageFlag mPublishTopics;
};