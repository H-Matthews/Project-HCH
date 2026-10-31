#pragma once

#include "core/ecs/BaseSystem.hpp"

namespace System {
/**
 * Draws the arena and every Shape in the world through a camera that follows the player.
 * Also draws health bars. HUD text is drawn by the owning state.
 */

class RenderSystem : public Core::BaseSystem {
  public:
    RenderSystem(sf::RenderWindow& window);

    void render() override;

  private:
    void updateCamera();
    void drawArena();
    void drawShapes();
    void drawHealthBars();

  private:
    sf::RenderWindow& mWindow;
};
} // namespace System
