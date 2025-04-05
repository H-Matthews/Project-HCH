#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "core/inc/Messaging/MessageNetwork.hpp"

#include "application/inc/Scene/ECS/EntityManager.hpp"

namespace Application
{
    /**
     * High Level class that represents a "Scene" during gameplay. It is used by the Game State to drive updates to our
     * Entities
     */
    class Scene
    {
      public:
        explicit Scene( sf::RenderWindow& window, Core::MessageNetwork& gameNetwork );

        void update( sf::Time fixedTimeStep );
        void draw();

      private:
        void init( Core::MessageNetwork& gameNetwork );
        void createEntity();

      private:
        sf::RenderWindow& mWindow;

        EntityManager mEntityManager;
    };
}
