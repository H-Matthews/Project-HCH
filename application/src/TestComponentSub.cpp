#include "TestComponentSub.hpp"


TestComponentSub::TestComponentSub(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub")
{
    mSubscribeToTopics.setFlag(MessageTopic::PLAYER);

    registerSubscriberTopics();
}

void TestComponentSub::onNotify(Message)
{
    std::cout << "Recieved Message in ComponentSub::onNotify for message topic for Player " << std::endl;
}