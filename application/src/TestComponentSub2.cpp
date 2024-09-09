#include "TestComponentSub2.hpp"
#include "EnemySpawnMessage.hpp"

TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    MessageNode::setSubscribeMessage(Messages::ID::EnemySpawnMessage);
    registerSubscriberMessages();
}

/* This class only handles the following Messages
1. PlayerActionMessage
*/
void TestComponentSub2::onNotify(Message* message)
{
    EnemySpawnMessage* eam = dynamic_cast<EnemySpawnMessage*>( message );

    if(eam != nullptr)
    {
        std::cout << "Receiver Node: " << MessageNode::getNodeName() << std::endl;
        std::cout << "Received Message: EnemyActionMessage " << std::endl;
        std::cout << "Sender: " << eam->getSenderName() << std::endl;
        std::cout << "Value: " << eam->x << std::endl;
    }
    std::cout << "\n";
}