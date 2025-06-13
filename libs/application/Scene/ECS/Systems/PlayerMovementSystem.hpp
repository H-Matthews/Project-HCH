#pragma once

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

namespace System
{
    /**
     * This Class iterates over all Entities with the PlayerInput & Sprite Component.
     * It takes the data in the PlayerInput component and applies it directly to the Sprite effectively applying the
     * Player Action to our Player Sprite
     */

    class PlayerMovementSystem : public BaseSystem
    {
      public:
        void update( const sf::Time timeStep ) override;
    };
}