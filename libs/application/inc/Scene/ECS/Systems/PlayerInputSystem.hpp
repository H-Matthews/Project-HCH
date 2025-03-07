#pragma once

#include "application/inc/Scene/ECS/Systems/BaseSystem.hpp"

#include "application/inc/MessageNetwork/Messages/PlayerInputMessage.hpp"
#include "core/inc/Messaging/MessageNode.hpp"

namespace System
{
    class PlayerInputSystem : public BaseSystem, public Core::MessageNode
    {
      public:
        PlayerInputSystem( Core::MessageNetwork& network );

      private:
        void onNotify( Core::Message* message ) override;
    };

}