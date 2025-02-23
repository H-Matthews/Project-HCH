#include "core/inc/Messaging/MessageNode.hpp"
#include "core/inc/Messaging/MessageNetwork.hpp"

#include <iostream>

Core::MessageNode::MessageNode(MessageNetwork& messageNetwork, const std::string& messageNodeName, NodeType nodeType) :
    mMessageNetwork(messageNetwork),
    mMessageNodeInfo(messageNodeName, this->getNotifyFunc()),
    mNodeType(nodeType),
    mNetworkLogger()
{   
    // Get Logger
    if constexpr (Utility::CAN_LOG)
        mNetworkLogger = Utility::LogRegistry::instance()->getLogger("MessageNetworkLogger");

    // Register Node
    switch(mNodeType)
    {
        case NodeType::PUBLISHER:
        {
            mMessageNetwork.registerPublisherNode(mMessageNodeInfo.nodeName);
            break;
        }
        case NodeType::SUBSCRIBER:
        {
            mMessageNetwork.registerSubscriberNode(mMessageNodeInfo.nodeName, mMessageNodeInfo.callback);
            break;
        }
        case NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.registerPublisherNode(mMessageNodeInfo.nodeName);
            mMessageNetwork.registerSubscriberNode(mMessageNodeInfo.nodeName, mMessageNodeInfo.callback);
            break;
        }
    }

}

// ONLY use this function if the node is BOTH a publisher and subscriber
// AND you wish to subscribe to the topic as a subscriber and publisher
// OTHERWISE, use the other functions addSubscriberTopic, addPublisherTopic
void Core::MessageNode::addTopic(Messages::ID messageID)
{
    switch(mNodeType)
    {
        case Core::NodeType::PUBLISHER:
        {
            mMessageNetwork.addPublisherTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
        case Core::NodeType::SUBSCRIBER:
        {
            mMessageNetwork.addSubscriberTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
        case Core::NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.addPublisherTopic(mMessageNodeInfo.nodeName, messageID);
            mMessageNetwork.addSubscriberTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
    }

   return;
}

void Core::MessageNode::addSubscriberTopic(Messages::ID messageID)
{
    mMessageNetwork.addSubscriberTopic(mMessageNodeInfo.nodeName, messageID);

    return;
}
void Core::MessageNode::addPublisherTopic(Messages::ID messageID)
{
    mMessageNetwork.addPublisherTopic(mMessageNodeInfo.nodeName, messageID);

    return;
}

// ONLY use this function if the node is BOTH a publisher and subscriber
// AND you wish to subscribe to the topic as a subscriber and publisher
// OTHERWISE, use the other functions addSubscriberTopic, addPublisherTopic
void Core::MessageNode::removeTopic(Messages::ID messageID)
{
    switch(mNodeType)
    {
        case Core::NodeType::PUBLISHER:
        {
            mMessageNetwork.addPublisherTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
        case Core::NodeType::SUBSCRIBER:
        {
            mMessageNetwork.addSubscriberTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
        case Core::NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.addPublisherTopic(mMessageNodeInfo.nodeName, messageID);
            mMessageNetwork.addSubscriberTopic(mMessageNodeInfo.nodeName, messageID);
            break;
        }
    }
}

void Core::MessageNode::removeSubscriberTopic(Messages::ID messageID)
{
    mMessageNetwork.removeTopicFromSubscriber(mMessageNodeInfo.nodeName, messageID);

    return;
}
void Core::MessageNode::removePublisherTopic(Messages::ID messageID)
{
    mMessageNetwork.removeTopicFromPublisher(mMessageNodeInfo.nodeName, messageID);

    return;
}

void Core::MessageNode::publish(std::shared_ptr<Message> message)
{ 
    message->setSender(mMessageNodeInfo.nodeName);

    if( message->getMessageID() != Messages::ID::NONE)
    {
        mMessageNetwork.publishMessage(message);
    }
    else
    {
        std::cout << "Did not send message because there is no Topic associated with NodeID: "
                  << mMessageNodeInfo.nodeName << std::endl;
    }
}

void Core::MessageNode::onNotify(Message*)
{
    std::cout << "Calling default method ---> MessageNode::onNotify(Message)... This message is intended for "
              << mMessageNodeInfo.nodeName << std::endl;
}

std::function<void (Core::Message*)> Core::MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [=, this] (Message* message) -> void {
        this->onNotify(message);
    };

    return messageSubscriber;
}