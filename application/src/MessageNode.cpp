#include "MessageNode.hpp"

#include <iostream>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mSubscriptionInfo(messageNodeName),
    mSubscribeToTopics(),
    mPublishToTopics()
{
    mSubscribeToTopics.reserve(MESSAGE_TOPIC_SIZE);
    mPublishToTopics.reserve(MESSAGE_TOPIC_SIZE);
    
    mSubscriptionInfo.callback = this->getNotifyFunc();
}

void MessageNode::registerSubscriberTopics()
{
    if(!mSubscribeToTopics.empty())
    {
        std::cout << "Registering Subscriber: " << mSubscriptionInfo.name << std::endl;

        std::pair<std::vector<MessageTopic >, MessageSubscriptionInfo> subscriberPair(mSubscribeToTopics, mSubscriptionInfo);
        mMessageNetwork->addSubscriber(subscriberPair);
    }
    else
    {
        std::cout << "Did not send message because Subscriber did not designate a topic to subscribe to " << std::endl;
    }
}

std::function<void (Message)> MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [=] (Message message) -> void {
        this->onNotify(message);
    };

    return messageSubscriber;
}

void MessageNode::build(Message& message)
{
    message.buildMessage(mPublishToTopics);
}

void MessageNode::send(Message message)
{ 
    // Assigns message the MessageNode::mPublishToTopics
    build(message);

    if( !message.getTopicList().empty())
    {
        std::cout << "Publishing Message. Sender: " << mSubscriptionInfo.name << std::endl;
        mMessageNetwork->sendMessage(message);
    }
    else
    {
        std::cout << "Did not send mesasge because there is no Topic associated with Node " << std::endl;
    }
}

void MessageNode::onNotify(Message)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mSubscriptionInfo.name << std::endl;
}