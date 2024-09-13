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
    auto pam = std::make_unique<PlayerActionMessage>(Message::ID::PlayerActionMessage);
    pam->action = Action::MOVE_DOWN;

    send(pam.get());

    auto esm = std::make_unique<EnemySpawnMessage>(Message::ID::EnemySpawnMessage);
    esm->spawn = true;

    send(esm.get());
}