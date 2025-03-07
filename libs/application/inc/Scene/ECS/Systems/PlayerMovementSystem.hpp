#pragma once

#include "application/inc/Scene/ECS/Systems/BaseSystem.hpp"

namespace System
{
    class PlayerMovementSystem : public BaseSystem
    {
      public:
        void update( const sf::Time timeStep ) override;
    };
}