#pragma once

#include "application/inc/Scene/ECS/Systems/BaseSystem.hpp"

#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"
#include "core/inc/Messaging/MessageNode.hpp"

namespace System
{
    /**
     * This Class is a subscriber on the Message Network that listens to PlayerActions.
     * When Receiving a PlayerAction Message, it manipulates the data in the PlayerInput Component
     * depending on the received Action
     */

    class PlayerInputSystem : public BaseSystem, public Core::MessageNode
    {
      public:
        PlayerInputSystem( Core::MessageNetwork& network );

      private:
        void onNotify( Core::Message* message ) override;
    };

}