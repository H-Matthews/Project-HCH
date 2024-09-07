#include "TestComponentSub.hpp"


TestComponentSub::TestComponentSub(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mSubscribeToTopics.push_back(MessageTopic::PLAYER);
    MessageNode::mSubscribeToTopics.push_back(MessageTopic::ENEMY);

    registerSubscriberTopics();
}

void TestComponentSub::onNotify(Message)
{
    std::cout << "Node: " << MessageNode::mSubscriptionInfo.name << " Received message " << std::endl;
}