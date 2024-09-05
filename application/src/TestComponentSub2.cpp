#include "TestComponentSub2.hpp"


TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub")
{
    mSubscribeToTopics.setFlag(MessageTopic::ENEMY);

    registerSubscriberTopics();
}

void TestComponentSub2::onNotify(Message)
{
    std::cout << "Recieved Message in ComponentSub2::onNotify for message topic for ENEMY " << std::endl;
}