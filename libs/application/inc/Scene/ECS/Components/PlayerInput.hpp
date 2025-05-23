#pragma once

#include <SFML/Graphics.hpp>

namespace Component
{
    struct PlayerInput
    {
        PlayerInput( sf::Vector2f playerVelocity );

        sf::Vector2f mPlayerVelocity;

        const float SPEED = 400.f;
    };

}