#pragma once

#include "application/inc/Messages/PlayerInputMessage.hpp"

#include "core/inc/Messaging/MessageNetwork.hpp"
#include "core/inc/Messaging/MessageNode.hpp"

class PlayerInput : public Core::MessageNode
{
    public:
        PlayerInput(Core::MessageNetwork* network);
        void update();

        std::shared_ptr<PlayerInputMessage> playerInputMessage;
};