#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Turns the intent accumulated in the PlayerInputComponent (see Scene::applyAction) into velocity,
 * facing and attack requests for the player entity, then clears it for the next step.
 */

class PlayerInputSystem : public Core::BaseSystem {
  public:
    void update(const sf::Time deltaTime) override;
};

} // namespace System
