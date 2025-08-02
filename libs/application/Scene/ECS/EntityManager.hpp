#pragma once

#include "vendor/entt/include/entt.hpp"

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/GameAssetContainer/ResourceEnums.hpp"

#include <SFML/Graphics.hpp>

namespace Application
{

    class Entity;

    /**
     * Main wrapper around the Entt Library. Maintains all of the Systems, Components, and Entities
     */

    class EntityManager
    {
      public:
        EntityManager( TextureHolder& textures );

        Entity createEntity();
        Entity createPlayerEntity( sf::Vector2f initialPosition, sf::Vector2f initialVelocity );

        void registerSystem( std::unique_ptr< System::BaseSystem > system );
        void registerRenderingSystem( std::unique_ptr< System::BaseSystem > renderSystem );

        void update( sf::Time timeStep );
        void render();

      private:
        entt::registry mEntityRegistry;

        std::vector< std::unique_ptr< System::BaseSystem > > systemsContainer;
        std::vector< std::unique_ptr< System::BaseSystem > > renderingSystemsContainer;

        // This is a reference passed from the top level class application
        TextureHolder& mTextures;

      public:
        friend class Entity;
    };
}