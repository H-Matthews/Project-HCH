#pragma once

#include "application/inc/Scene/ECS/EntityManager.hpp"

namespace Application
{

    /**
     * This Class serves as a wrapper around the entt::entity type so that we don't have to operate directly with the
     * Entt library
     */

    class Entity
    {
      public:
        Entity();

        inline entt::entity getHandle()
        {
            return mEntityHandle;
        }

        template < typename T, typename... Args > T& addComponent( Args&&... args )
        {
            return mEntityManager->mEntityRegistry.emplace< T >( mEntityHandle, std::forward< Args >( args )... );
        }

        template < typename T > T& getComponent()
        {
            return mEntityManager->mEntityRegistry.get< T >( mEntityHandle );
        }

        template < typename T > void removeComponent()
        {
            return mEntityManager->mEntityRegistry.remove< T >( mEntityHandle );
        }

        template < typename T > bool hasComponent()
        {
            return mEntityManager->mEntityRegistry.all_of< T >( mEntityHandle );
        }

      private:
        Entity( entt::entity entityHandle, EntityManager* entityManager );

      private:
        entt::entity mEntityHandle = entt::entity( 0 );
        EntityManager* mEntityManager = nullptr;

      public:
        friend class EntityManager;
    };
}