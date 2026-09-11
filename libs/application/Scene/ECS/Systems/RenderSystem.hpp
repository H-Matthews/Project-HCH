#pragma once

#include "application/Scene/ECS/Systems/BaseSystem.hpp"

namespace System {
/**
 * Draws all Sprite Components to the SFML Window
 */

class RenderSystem : public BaseSystem {
  public:
    RenderSystem(sf::RenderWindow& window);

    void render() override;

  private:
    sf::RenderWindow& mWindow;
};
} // namespace System