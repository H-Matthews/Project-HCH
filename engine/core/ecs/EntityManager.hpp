#pragma once

#include "vendor/entt/include/entt.hpp"

#include "core/ecs/BaseSystem.hpp"

#include <SFML/Graphics.hpp>

namespace Core {

class Entity;

/**
 * Main wrapper around the Entt Library. Maintains all of the Systems, Components, and Entities
 */

class EntityManager {
  public:
    EntityManager() = default;

    Entity createEntity();

    entt::registry& registry() { return mEntityRegistry; }
    const entt::registry& registry() const { return mEntityRegistry; }

    void registerSystem(std::unique_ptr<BaseSystem> system);
    void registerRenderingSystem(std::unique_ptr<BaseSystem> renderSystem);

    void update(sf::Time timeStep);
    void render();

  private:
    entt::registry mEntityRegistry;

    std::vector<std::unique_ptr<BaseSystem>> systemsContainer;
    std::vector<std::unique_ptr<BaseSystem>> renderingSystemsContainer;

  public:
    friend class Entity;
};
} // namespace Core