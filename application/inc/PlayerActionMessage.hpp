#pragma once

#include "Message.hpp"

// Test Class
enum class Action
{
    MOVE_UP = 0,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};

class PlayerActionMessage : public Message 
{
    public:
        PlayerActionMessage( const Message::ID messageID );

        Action action;
};