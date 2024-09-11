#include "TestComponentPub.hpp"

#include <iostream>
#include <memory>

TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork)
{
    MessageNode::mMessageNodeInfo.nodeName = "TestComponentPublish";
}

void TestComponentPub::update()
{
    auto pamBase = createMessage(Message::ID::PlayerActionMessage);

    // Base to derived cast 
    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( pamBase.get() );
    if(pam)
    {
        pam->action = Action::MOVE_DOWN;
    }

    send(pamBase.get());

    auto esmBase = createMessage(Message::ID::EnemySpawnMessage);

    EnemySpawnMessage* esm = dynamic_cast<EnemySpawnMessage*>( esmBase.get() );
    if(esm)
    {
        esm->spawn = true;
    }

    send(esmBase.get());
}