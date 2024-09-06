#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork, "TestComponentPublish")
{
    // Set subscribe / Publish  topics here
    MessageNode::mPublishToTopics.setFlag(MessageTopic::PLAYER);
    MessageNode::mPublishToTopics.setFlag(MessageTopic::ENEMY);
    //MessageNode::mPublishToTopics.setFlag(MessageTopic::DROPS);
}

void TestComponentPub::update()
{
    Message message;

    // You can set the mPublishTopics flag based on logic as well
    // This will publish a message for both PLAYER & ENEMY
    // mPublishToTopics.setFlag(MessageTopic::ENEMY);
    send(message);
}