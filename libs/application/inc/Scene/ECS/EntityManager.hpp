#pragma once

#include "vendor/entt/include/entt.hpp"

#include "application/inc/Scene/ECS/Systems/BaseSystem.hpp"

#include <SFML/Graphics.hpp>

namespace Application
{

    class Entity;

    class EntityManager
    {
      public:
        Entity createEntity();

        void registerSystem( std::unique_ptr< System::BaseSystem > system );
        void registerRenderingSystem( std::unique_ptr< System::BaseSystem > renderSystem );

        void update( sf::Time timeStep );
        void render();

      private:
        entt::registry mEntityRegistry;

        std::vector< std::unique_ptr< System::BaseSystem > > systemsContainer;
        std::vector< std::unique_ptr< System::BaseSystem > > renderingSystemsContainer;

      public:
        friend class Entity;
    };
}