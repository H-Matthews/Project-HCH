#include "application/Scene/ECS/Systems/PlayerMovementSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"

#include <cmath>

void System::PlayerMovementSystem::update( const sf::Time timeStep )
{
    auto view = mRegistry->view< Component::PlayerInput, Component::Sprite >();

    // Iterate over all sprites with playerInput component (AKA our PLAYER)
    for ( auto entity : view )
    {
        auto& playerInputData = view.get< Component::PlayerInput >( entity );
        auto& sprite = view.get< Component::Sprite >( entity );

        sprite.mVelocity += playerInputData.mPlayerVelocity;

        // IF WE MOVE DIAGONALLY
        if ( sprite.mVelocity.x != 0.f && sprite.mVelocity.y != 0.f )
        {
            // TODO: Get MASTER PHYSICS LORD to change this
            sprite.mVelocity /= std::sqrt( 2.f );
        }

        sprite.mRectangleShape.move( sprite.mVelocity * timeStep.asSeconds() );

        // Reset both Velocitys after performing the move
        sprite.mVelocity = sf::Vector2f( { 0.f, 0.f } );
        playerInputData.mPlayerVelocity = sf::Vector2f( { 0.f, 0.f } );
    }

    return;
}