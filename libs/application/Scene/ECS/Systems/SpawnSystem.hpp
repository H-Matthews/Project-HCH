#pragma once

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

#include <random>

namespace System {
/**
 * Spawns enemy waves. A new, larger wave starts shortly after the previous one is cleared.
 */
class SpawnSystem : public BaseSystem {
  public:
    SpawnSystem();

    void update(const sf::Time deltaTime) override;

  private:
    sf::Vector2f pickSpawnPosition(sf::Vector2f playerPosition);

  private:
    std::mt19937 mRng;
};
} // namespace System
