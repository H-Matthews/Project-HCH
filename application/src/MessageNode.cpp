#include "MessageNode.hpp"

#include <iostream>
#include <cassert>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo(messageNodeName)
{   
    mMessageNodeInfo.callback = this->getNotifyFunc();
}

MessageNode::MessageNode(MessageNetwork* messageNetwork) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo("")
{
}

std::unique_ptr<Message> MessageNode::createMessage(Message::ID messageID)
{
    auto found = mMessageNetwork->getMessageRegistry().find(messageID);
    assert(found != mMessageNetwork->getMessageRegistry().end());

    // Returns the newly created Message Object
    return found->second();
}

void MessageNode::subscribeTo(Message::ID subscribeMessageID)
{
    auto result = mMessageNodeInfo.subscriptions.insert(subscribeMessageID);
    mMessageNodeInfo.stringSubscriberList.insert(Utility::messageEnumToString(subscribeMessageID));

    if(!result.second)
        std::cout << "Already subscribing to that message: " << Utility::messageEnumToString(subscribeMessageID) << std::endl;
}

void MessageNode::registerSubscriberMessages()
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

std::function<void (Message*)> MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [=] (Message* message) -> void {
        this->onNotify(message);
    };

    return messageSubscriber;
}

void MessageNode::send(Message* message)
{ 
    message->setSender(mMessageNodeInfo.nodeName);

    if( message->getMessageID() != Message::ID::NONE)
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

void MessageNode::onNotify(Message*)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mMessageNodeInfo.nodeName << std::endl;
}