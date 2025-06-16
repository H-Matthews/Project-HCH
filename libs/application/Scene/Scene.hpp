#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "core/Messaging/MessageNetwork.hpp"
#include "core/GameAssetContainer/ResourceEnums.hpp"

#include "application/Scene/ECS/EntityManager.hpp"

namespace Application
{

    /**
     * High Level class that represents a "Scene" during gameplay. It is used by the Game State to drive updates to our
     * Entities
     */
    class Scene
    {
      public:
        explicit Scene( sf::RenderWindow& window, Core::MessageNetwork& gameNetwork, TextureHolder& textures );

        void update( sf::Time fixedTimeStep );
        void draw();

      private:
        void initializeGameSystems( Core::MessageNetwork& gameNetwork );
        void initializePlayerEntity();

      private:
        sf::RenderWindow& mWindow;

        EntityManager mEntityManager;
    };
}
