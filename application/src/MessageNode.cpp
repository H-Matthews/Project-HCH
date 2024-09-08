#include "MessageNode.hpp"

#include <iostream>
#include <cassert>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo(messageNodeName),
    mSubscribeToMessages(),
    mPublishMessageID()
{   
    mMessageNodeInfo.callback = this->getNotifyFunc();
}

MessageNode::MessageNode(MessageNetwork* messageNetwork) :
    mMessageNetwork(messageNetwork)
{
}

std::unique_ptr<Message> MessageNode::createMessage(Messages::ID messageID)
{
    auto found = mMessageNetwork->getMessageRegistry().find(messageID);
    assert(found != mMessageNetwork->getMessageRegistry().end());

    // Returns the newly created Message Object
    return found->second();
}

void MessageNode::setPublishMessage(Messages::ID publishMessageID)
{
    mPublishMessageID = publishMessageID;
}

void MessageNode::setSubscribeMessage(Messages::ID subscribeMessageID)
{
    auto result = mSubscribeToMessages.insert(subscribeMessageID);

    if(!result.second)
        std::cout << "Already subscribing to that message: " << Utility::messageEnumToString(subscribeMessageID) << std::endl;
}

void MessageNode::setNodeName(std::string nodeName)
{
    mMessageNodeInfo.nodeName = nodeName;
}

const std::set< Messages::ID >& MessageNode::getSubscribeToMessageList() const
{
    return mSubscribeToMessages;
}

const Messages::ID& MessageNode::getPublishMessageID() const
{
    return mPublishMessageID;
}

const std::string& MessageNode::getNodeName() const
{
    return mMessageNodeInfo.nodeName;
}

void MessageNode::registerSubscriberMessages()
{
    if(!mSubscribeToMessages.empty())
    {
        std::cout << "Registering Subscriber: " << mMessageNodeInfo.nodeName << std::endl;

        std::pair<std::set< Messages::ID >, MessageNodeInfo> subscriberPair(mSubscribeToMessages, mMessageNodeInfo);
        mMessageNetwork->addSubscriber(subscriberPair);
    }
    else
    {
        std::cout << "Did not Register Subscriber because they did not designate a topic to subscriber to " << std::endl;
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
    message->populateMessageHeader(mPublishMessageID, mMessageNodeInfo.nodeName);

    if( mPublishMessageID != Messages::ID::None)
    {
        std::cout << "Publishing Message. Sender: " << mMessageNodeInfo.nodeName << std::endl;
        mMessageNetwork->sendMessage(message);
    }
    else
    {
        std::cout << "Did not send message because there is no Topic associated with NodeID: " << getNodeName() << std::endl;
    }

    message = nullptr;
}

void MessageNode::onNotify(Message*)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mMessageNodeInfo.nodeName << std::endl;
}