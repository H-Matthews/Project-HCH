#pragma once

#include "vendor/entt/include/entt.hpp"

#include <SFML/Graphics.hpp>

namespace System
{
    /**
     * Base class for all Systems
     *
     * Allows us to have a reference to the registry for every system.
     *
     */
    class BaseSystem
    {
      public:
        void init( entt::registry& registry );

        virtual void update( const sf::Time )
        {
            return;
        }

        virtual void render()
        {
            return;
        }

      protected:
        entt::registry* mRegistry;
    };
}