#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork, "TestComponentPublish")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mPublishToTopics.push_back(MessageTopic::PLAYER);
    MessageNode::mPublishToTopics.push_back(MessageTopic::ENEMY);
}

void TestComponentPub::update()
{
    Message message;

    // You can set the mPublishTopics flag based on logic as well
    // This will publish a message for both PLAYER & ENEMY
    // mPublishToTopics.setFlag(MessageTopic::ENEMY);
    send(message);
}