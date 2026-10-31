#pragma once

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

namespace System {
/**
 * Ticks damage-immunity timers, removes dead enemies (awarding score) and flags player death in
 * the shared GameStats.
 */
class HealthSystem : public BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};
} // namespace System
