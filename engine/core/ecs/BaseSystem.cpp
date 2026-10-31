#include "core/ecs/BaseSystem.hpp"

void Core::BaseSystem::init(entt::registry& registry) {
    mRegistry = &registry;
}