#pragma once

#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace System::Math {
inline float length(sf::Vector2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

// Returns a zero vector for (near) zero input, so callers never divide by zero
inline sf::Vector2f normalized(sf::Vector2f v) {
    const float len = length(v);
    return len > 0.0001f ? sf::Vector2f{v.x / len, v.y / len} : sf::Vector2f{0.f, 0.f};
}
} // namespace System::Math
