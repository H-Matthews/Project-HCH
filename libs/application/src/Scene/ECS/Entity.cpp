#include "application/inc/Scene/ECS/Entity.hpp"

Application::Entity::Entity( entt::entity entityHandle, EntityManager* entityManager ) :
    mEntityHandle( entityHandle ),
    mEntityManager( entityManager )
{}