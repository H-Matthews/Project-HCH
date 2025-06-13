#include "application/Scene/ECS/Systems/BaseSystem.hpp"

void System::BaseSystem::init( entt::registry& registry )
{
    mRegistry = &registry;
}