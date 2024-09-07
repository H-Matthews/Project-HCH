#include "TestComponentSub2.hpp"


TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mSubscribeToTopics.push_back(MessageTopic::PLAYER);
    MessageNode::mSubscribeToTopics.push_back(MessageTopic::ENEMY);
    MessageNode::mSubscribeToTopics.push_back(MessageTopic::DROPS);

    registerSubscriberTopics();
}

void TestComponentSub2::onNotify(Message)
{
    std::cout << "Node: " << MessageNode::mSubscriptionInfo.name << " Received message " << std::endl;
}