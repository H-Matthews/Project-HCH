#include "MessageNode.hpp"

#include <iostream>

MessageNode::MessageNode(MessageNetwork* messageNetwork, const std::string& messageNodeName) :
    mMessageNetwork(messageNetwork),
    mSubscriptionInfo(messageNodeName),
    mSubscribeToTopics(),
    mPublishToTopics()
{
    mSubscriptionInfo.callback = this->getNotifyFunc();
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
   message.setMessageTopicFlag(mPublishToTopics);
   message.populateTopicList();

   if( message.getTopicFlag().getFlagValue() != 0 )
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

void MessageNode::registerSubscriberTopics()
{
    if(mSubscribeToTopics.getFlagValue() != 0)
    {
        std::cout << "Registering Subscriber: " << mSubscriptionInfo.name << std::endl;

        std::pair<MessageTopicFlag, MessageSubscriptionInfo> subscriberPair(mSubscribeToTopics, mSubscriptionInfo);
        mMessageNetwork->addSubscriber(subscriberPair);
    }
    else
    {
        std::cout << "Did not send message because Subscriber did not designate a topic to subscribe to " << std::endl;
    }
}