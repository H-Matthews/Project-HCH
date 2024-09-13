#include "TestComponentPub.hpp"

#include <iostream>
#include <memory>

TestComponentPub::TestComponentPub(MessageNetwork* messageNetwork) : 
    MessageNode(messageNetwork),
    playerAction()
{
    MessageNode::mMessageNodeInfo.nodeName = "TestComponentPublish";

    // Initialize Messages
    playerAction = std::make_unique<PlayerActionMessage>(Message::ID::PlayerActionMessage);
    enemySpawn = std::make_unique<EnemySpawnMessage>(Message::ID::EnemySpawnMessage);
}

void TestComponentPub::update()
{
    playerAction->action = Action::MOVE_DOWN;

    send(playerAction.get());

    enemySpawn->spawn = true;

    send(enemySpawn.get());
}