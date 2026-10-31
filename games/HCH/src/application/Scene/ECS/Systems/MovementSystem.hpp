#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Integrates velocity + knockback into position, decays knockback and keeps entities inside the
 * world.
 */
class MovementSystem : public Core::BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};
} // namespace System
