#include "core/ecs/Entity.hpp"

Core::Entity::Entity(entt::entity entityHandle, EntityManager* entityManager)
    : mEntityHandle(entityHandle), mEntityManager(entityManager) {}