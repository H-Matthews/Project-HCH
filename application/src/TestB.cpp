#include "TestB.hpp"


TestB::TestB(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork)
{
    MessageFlag publishTopics(0);
    publishTopics.setFlag(MessageTopics::PLAYER);
    publishTopics.setFlag(MessageTopics::ENEMY);

    mPublishTopics = publishTopics;

    MessageNode::registerMessageListener();
}

void TestB::onNotify(Message message)
{
    std::cout << "Do nothing" << std::endl;
}

void TestB::update()
{
    Message message(mPublishTopics, MessageType::Movement);
    send(message);
}