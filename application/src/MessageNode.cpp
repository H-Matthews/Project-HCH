#include "MessageNode.hpp"

#include <iostream>

MessageNode::MessageNode(MessageNetwork* messageNetwork) :
    mMessageNetwork(messageNetwork),
    mSubscribeTopics(0),
    mPublishTopics(0)
{
}

void MessageNode::registerMessageListener()
{
    // Only Register as a listener if we are listening....
    if(mSubscribeTopics.getFlagValue() != 0)
        mMessageNetwork->addListener(this->getNotifyFunc());
}

std::function<void (Message)> MessageNode::getNotifyFunc()
{
    auto messageListener = [=] (Message message) -> void {
        this->onNotify(message);
    };

    return messageListener;
}

void MessageNode::send(Message message)
{
    // Only send messages if were publishing
    if(mPublishTopics.getFlagValue() != 0)
        mMessageNetwork->addMessage(message);
}

void MessageNode::onNotify(Message message)
{
}