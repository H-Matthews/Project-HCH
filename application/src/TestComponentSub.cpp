#include "TestComponentSub.hpp"


TestComponentSub::TestComponentSub(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub")
{
    MessageNode::mSubscribeToTopics.setFlag(MessageTopic::PLAYER);
    MessageNode::mSubscribeToTopics.setFlag(MessageTopic::ENEMY);

    registerSubscriberTopics();
}

void TestComponentSub::onNotify(Message)
{
    std::cout << "Node: " << MessageNode::mSubscriptionInfo.name << " Recieved message "
              << "for topic PLAYER " << std::endl;
}