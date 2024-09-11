#pragma once

#include "Message.hpp"

#include <string>

// Test Class
enum class Action
{
    MOVE_UP = 0,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT
};

struct PlayerActionMessage : public Message 
{
    PlayerActionMessage( const std::pair< Message::ID, std::string >& messageID );
    PlayerActionMessage( PlayerActionMessage* message);

    Action action;
};