#pragma once

#include "Message.hpp"

#include <string>

struct EnemySpawnMessage : public Message 
{
    EnemySpawnMessage( const std::pair< Message::ID, std::string >& messageID );
    EnemySpawnMessage( EnemySpawnMessage* message);

    bool spawn;
};