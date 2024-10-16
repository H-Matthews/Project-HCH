#include "core/inc/Messaging/MessageNode.hpp"
#include "core/inc/Messaging/MessageNetwork.hpp"

#include <iostream>

Core::MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo(messageNodeName)
{   
    mMessageNodeInfo.callback = this->getNotifyFunc();
}

Core::MessageNode::MessageNode(MessageNetwork* messageNetwork) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo("")
{
}

void Core::MessageNode::subscribeTo(Messages::ID subscribeMessageID)
{
    auto result = mMessageNodeInfo.subscriptions.insert(subscribeMessageID);

    if(!result.second)
        std::cout << "Already Subscribed to that Message " << std::endl;
}

// Notifys messageNetwork that we would like to unSubscribe from a message
// It will evaulate the request at the end of MessageNetwork::notifySubscribers()
// This ensures the messages that were sent in the middle of the loop still gets sent to 
// the subscriber as intended
void Core::MessageNode::notifyUnsubscribe(Messages::ID messageID)
{
    mMessageNetwork->insertUnsubscriber(messageID, mMessageNodeInfo.nodeName);
}

void Core::MessageNode::registerSubscriberMessages()
{
    if(!mMessageNodeInfo.subscriptions.empty())
    {
        std::cout << "Registering Subscriber: " << mMessageNodeInfo.nodeName << std::endl;
        mMessageNetwork->addSubscriber(mMessageNodeInfo);
    }
    else
    {
        std::cout << mMessageNodeInfo.nodeName << " has not set any messages to subscribe to " << std::endl;
    }
}

void Core::MessageNode::send(Message* message)
{ 
    message->setSender(mMessageNodeInfo.nodeName);

    if( message->getMessageID() != Messages::ID::NONE)
    {
        std::cout << "Publishing Message. Sender: " << mMessageNodeInfo.nodeName << std::endl;
        mMessageNetwork->sendMessage(message);
    }
    else
    {
        std::cout << "Did not send message because there is no Topic associated with NodeID: "
                  << mMessageNodeInfo.nodeName << std::endl;
    }

    message = nullptr;
}

void Core::MessageNode::onNotify(Message*)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mMessageNodeInfo.nodeName << std::endl;
}

std::function<void (Core::Message*)> Core::MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [=] (Message* message) -> void {
        this->onNotify(message);
    };

    return messageSubscriber;
}