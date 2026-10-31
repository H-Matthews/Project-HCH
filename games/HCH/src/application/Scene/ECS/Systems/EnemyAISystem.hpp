#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Enemies walk toward the player, keep a little spacing from each other and request a melee
 * attack once they are close enough.
 */
class EnemyAISystem : public Core::BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};
} // namespace System
