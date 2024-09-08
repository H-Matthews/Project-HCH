#pragma once

#include "Message.hpp"

#include <string>

// Test Class
enum Action
{
    MOVE_UP,
    MOVE_DOWN

};

class PlayerActionMessage : public Message 
{
    public:
        PlayerActionMessage();
        PlayerActionMessage( MessageNetwork& messageNetwork, std::string identifierString);
        PlayerActionMessage( PlayerActionMessage* message);

        Action action = MOVE_UP;
};