#pragma once

#include <SFML/Graphics.hpp>

namespace Component
{
    struct Sprite
    {
        Sprite( sf::RectangleShape shape, sf::Vector2f position, sf::Vector2f velocity );

        sf::RectangleShape mRectangleShape;
        sf::Vector2f mPosition;
        sf::Vector2f mVelocity;
    };
}