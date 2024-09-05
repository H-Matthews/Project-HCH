#include "TestComponentPub.hpp"


TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork, "TestComponentPublish")
{
    // Set subscribe / Publish  topics here
    mPublishToTopics.setFlag(MessageTopic::PLAYER);
}

void TestComponentPub::update()
{
    Message message(mPublishToTopics);
    std::cout << "Publishing Message from ComponentPub::update() " << std::endl;
    send(message);
}