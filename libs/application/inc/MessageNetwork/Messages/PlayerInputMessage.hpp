#pragma once

#include "core/inc/Messaging/Message.hpp"

#include "application/inc/PlayerAction.hpp"

namespace Application
{
    struct PlayerInputMessage : public Core::Message
    {
        PlayerInputMessage( const Messages::ID messageID );
        PlayerInputMessage* clone() const override;

        Actions::Player action;
    };
}