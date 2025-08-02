#include "application/Scene/ECS/Systems/RenderSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"

System::RenderSystem::RenderSystem( sf::RenderWindow& window ) :
    mWindow( window )
{}

void System::RenderSystem::render()
{
    // Draw Background first

    auto view = mRegistry->view< Component::SpriteComponent >();

    for ( auto entity : view )
    {
        auto& sprite = view.get< Component::SpriteComponent >( entity );
        mWindow.draw( sprite.mSprite );
    }
}