#pragma once

#include "core/inc/Messaging/Message.hpp"

namespace Application
{
    enum class Action
    {
        NONE = 0,
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT
    };


    struct PlayerInputMessage : public Core::Message
    {
        PlayerInputMessage(const Messages::ID messageID);
        Action action;
    };
}