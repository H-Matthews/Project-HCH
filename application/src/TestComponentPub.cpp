#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork, "TestComponentPublish")
{
    // Set Subscribe / Publish  topics here
    MessageNode::mPublishToTopics.push_back(MessageTopic::PLAYER);
    MessageNode::mPublishToTopics.push_back(MessageTopic::ENEMY);
    MessageNode::mPublishToTopics.push_back(MessageTopic::DROPS);
}

void TestComponentPub::update()
{
    std::unique_ptr< Message > message = std::make_unique< Message >();

    // You can dynamically add more mPublishToTopics if needed
    // MessageNode::mPublishToTopics.setFlag(MessageTopic::ENEMY);

    send(message.get());
}