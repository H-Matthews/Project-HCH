#pragma once

#include <SFML/System/Vector2.hpp>

namespace Component {
/**
 * Raw player intent collected during a step (see Scene::applyAction).
 * PlayerInputSystem converts it into velocity / attack requests, then clears it.
 */
struct PlayerInputComponent {
    sf::Vector2f direction{0.f, 0.f}; // Sum of held movement keys (each axis -1..1)
    bool attackRequested = false;
};

} // namespace Component
