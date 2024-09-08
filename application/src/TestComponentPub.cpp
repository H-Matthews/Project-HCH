#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork)
{
    MessageNode::setPublishMessage(Messages::ID::PlayerActionMessage);
    MessageNode::setNodeName("TestComponentPublish");
}

void TestComponentPub::update()
{
    auto msg = createMessage(Messages::ID::PlayerActionMessage);

    // You can dynamically add more mPublishToTopics if needed
    // MessageNode::mPublishToTopics.push_back(MessageTopic::DROPS);

    send(msg.get());
}