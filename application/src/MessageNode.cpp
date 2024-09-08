#include "MessageNode.hpp"

#include <iostream>
#include <cassert>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mSubscriptionInfo(messageNodeName),
    mSubscribeToTopics(),
    mPublishToTopics()
{
    mSubscribeToTopics.reserve(3);
    mPublishToTopics.reserve(3);
    
    mSubscriptionInfo.callback = this->getNotifyFunc();
}

std::unique_ptr<Message> MessageNode::createMessage(Messages::ID messageID)
{
    auto found = mMessageNetwork->getMessageRegistry().find(messageID);
    assert(found != mMessageNetwork->getMessageRegistry().end());

    // Returns the newly created Message Object
    return found->second();
}

void MessageNode::registerSubscriberTopics()
{
    if(!mSubscribeToTopics.empty())
    {
        std::cout << "Registering Subscriber: " << mSubscriptionInfo.name << std::endl;

        std::pair<std::vector< Messages::ID >, MessageSubscriptionInfo> subscriberPair(mSubscribeToTopics, mSubscriptionInfo);
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
    message->buildMessage(mPublishToTopics, mPublisherName);

    if( !message->getTopicList().empty())
    {
        std::cout << "Publishing Message. Sender: " << mSubscriptionInfo.name << std::endl;
        mMessageNetwork->sendMessage(message);
    }
    else
    {
        std::cout << "Did not send message because there is no Topic associated with Node " << std::endl;
    }

    message = nullptr;
}

void MessageNode::onNotify(Message*)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mSubscriptionInfo.name << std::endl;
}