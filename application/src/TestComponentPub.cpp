#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork)
{
    MessageNode::setNodeName("TestComponentPublish");
}

void TestComponentPub::update()
{
    auto msg = createMessage(Messages::ID::PlayerActionMessage);

    MessageNode::setPublishMessage(Messages::ID::PlayerActionMessage);
    send(msg.get());

    msg = createMessage(Messages::ID::EnemySpawnMessage);

    MessageNode::setPublishMessage(Messages::ID::EnemySpawnMessage);
    send(msg.get());
}