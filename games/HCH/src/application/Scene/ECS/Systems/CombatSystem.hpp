#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Turns attack requests into short-lived hitboxes and applies hitbox damage / knockback to
 * entities on the opposing team.
 */
class CombatSystem : public Core::BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;

  private:
    void spawnHitboxes(float dt);
    void resolveHitboxes(float dt);
};
} // namespace System
