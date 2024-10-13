#pragma once

#include "Message.hpp"

class EnemySpawnMessage : public Message 
{
    public:
        EnemySpawnMessage( const Message::ID messageID );

        bool spawn;
};