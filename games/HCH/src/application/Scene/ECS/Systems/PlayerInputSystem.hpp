#pragma once

#include "core/ecs/BaseSystem.hpp"

#include "application/Messaging/Messages/PlayerInputMessage.hpp"

#include "core/Messaging/MessageNode.hpp"

namespace System {
/**
 * Subscriber on the Message Network that listens to PlayerActions. Messages accumulate intent in
 * the PlayerInputComponent; update() then turns that intent into velocity, facing and attack
 * requests for the player entity and clears it for the next step.
 */

class PlayerInputSystem : public Core::BaseSystem, public Core::MessageNode {
  public:
    PlayerInputSystem(Core::MessageNetwork& network);

    void update(const sf::Time deltaTime) override;

  private:
    void onNotify(Core::Message* message) override;
};

} // namespace System
