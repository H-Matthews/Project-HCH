#pragma once

#include <SFML/Graphics.hpp>

namespace Component
{
    struct SpriteComponent
    {
        SpriteComponent( sf::Sprite sprite, sf::Vector2f position, sf::Vector2f velocity );

        sf::Sprite mSprite;
        sf::Vector2f mPosition;
        sf::Vector2f mVelocity;
    };
}