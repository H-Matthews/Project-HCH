#pragma once

#include<Message.hpp>

struct EnemySpawnMessage : public Message 
{
    EnemySpawnMessage();
    EnemySpawnMessage( MessageNetwork& messageNetwork, std::string identifierString);
    EnemySpawnMessage( EnemySpawnMessage* message);

    int x = 100;
};