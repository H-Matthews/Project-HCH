#pragma once

#include "Message.hpp"

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
        PlayerActionMessage( PlayerActionMessage* message);

        void buildMessage(std::vector< MessageTopic > messageTopics, std::string sender) override;

        Action action = MOVE_UP;
};