#include "application/Scene/ECS/Components/Sprite.hpp"

Component::Sprite::Sprite( sf::RectangleShape shape, sf::Vector2f position, sf::Vector2f velocity ) :
    mRectangleShape( shape ),
    mPosition( position ),
    mVelocity( velocity )
{
    // Set Initial Position
    mRectangleShape.setPosition( mPosition );
}