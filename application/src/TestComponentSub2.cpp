#include "TestComponentSub2.hpp"

TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    MessageNode::subscribeTo(Message::ID::EnemySpawnMessage);
    registerSubscriberMessages();
}

/* This class only handles the following Messages
1. PlayerActionMessage
*/
void TestComponentSub2::onNotify(Message* message)
{
    EnemySpawnMessage* pam = dynamic_cast<EnemySpawnMessage*>( message );
    if(pam)
    {
        if(pam->spawn == true)
            std::cout << "Spawning Enemy...... " << std::endl;

        std::cout << pam->getStringMessageID() << std::endl;
    }
}