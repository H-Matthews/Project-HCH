#include "MessageNode.hpp"

#include <iostream>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mSubscribeToTopics(),
    mPublishToTopics(),
    mMessageNodeName(messageNodeName)
{
}

std::function<void (Message)> MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [=] (Message message) -> void {
        this->onNotify(message);
    };

    return messageSubscriber;
}

void MessageNode::send(Message message)
{
    // Only send messages if were publishing
    if(mPublishToTopics.getFlagValue() != 0)
        mMessageNetwork->sendMessage(message);
}

void MessageNode::onNotify(Message)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... Maybe forgot to implement??" << std::endl;
}

void MessageNode::registerSubscriberTopics()
{
    // Only Register as a listener if we are listening....
    if(mSubscribeToTopics.getFlagValue() != 0)
        mMessageNetwork->addSubscriber(mSubscribeToTopics, this->getNotifyFunc());
}