#include "TestComponentSub2.hpp"

TestComponentSub2::TestComponentSub2(MessageNetwork* messageNetwork) : 
        MessageNode(messageNetwork, "TestComponentSub2")
{
    MessageNode::subscribeTo(Message::ID::EnemySpawnMessage);
    MessageNode::subscribeTo(Message::ID::PlayerActionMessage);
    registerSubscriberMessages();
}

void TestComponentSub2::onNotify(Message* message)
{

    std::cout << MessageNode::mMessageNodeInfo.nodeName << " recieved " << message->getStringMessageID() << std::endl;
    EnemySpawnMessage* esm = dynamic_cast<EnemySpawnMessage*>( message );
    if(esm)
    {
        if(esm->spawn == true)
            std::cout << "Spawning Enemy...... " << std::endl;
    }

    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( message );
    if(pam)
    {
        if(pam->action == Action::MOVE_DOWN)
            std::cout << "Moving Down " << std::endl;
    }

    MessageNode::notifyUnsubscribe(Message::ID::EnemySpawnMessage);
    MessageNode::notifyUnsubscribe(Message::ID::PlayerActionMessage);

    std::cout << "\n";
}