#pragma once

#include "core/Messaging/Message.hpp"

#include "application/Data/PlayerAction.hpp"

namespace Application
{
    struct PlayerInputMessage : public Core::Message
    {
        PlayerInputMessage( const Messages::ID messageID );
        PlayerInputMessage* clone() const override;

        Actions::Player action;
    };
}