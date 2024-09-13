#pragma once

#include "Message.hpp"

class EnemySpawnMessage : public Message 
{
    public:
        EnemySpawnMessage( const Message::ID messageID );
        std::unique_ptr<Message> clone() const override;

        bool spawn;
};