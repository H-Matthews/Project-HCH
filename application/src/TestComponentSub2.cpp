#include "TestComponentSub2.hpp"


TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    MessageNode::mSubscribeToTopics.setFlag(MessageTopic::PLAYER);
    MessageNode::mSubscribeToTopics.setFlag(MessageTopic::ENEMY);

    registerSubscriberTopics();
}

void TestComponentSub2::onNotify(Message)
{
    std::cout << "Node: " << MessageNode::mSubscriptionInfo.name << " Received message "
              << "for topic ENEMY " << std::endl;
}