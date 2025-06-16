#include "application/Scene/ECS/Components/SpriteComponent.hpp"

#include <SFML/Graphics.hpp>

Component::SpriteComponent::SpriteComponent( sf::Sprite sprite, sf::Vector2f position, sf::Vector2f velocity ) :
    mSprite( sprite ),
    mPosition( position ),
    mVelocity( velocity )
{
    // Set Initial Position
    mSprite.setPosition( mPosition );

    // Sets the origin to the middle of the sprite
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin( { bounds.size.x, bounds.size.y } );

    mSprite.setScale( { 2.f, 2.f } );
}