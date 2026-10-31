#pragma once

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

namespace System {
/**
 * Integrates velocity + knockback into position, decays knockback and keeps entities inside the
 * world.
 */
class MovementSystem : public BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};
} // namespace System
