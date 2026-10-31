#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Ticks damage-immunity timers, removes dead enemies (awarding score) and flags player death in
 * the shared GameStats.
 */
class HealthSystem : public Core::BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};
} // namespace System
