#include "application/inc/Scene/ECS/Systems/RenderSystem.hpp"

#include "application/inc/Scene/ECS/Components/Components.hpp"

System::RenderSystem::RenderSystem( sf::RenderWindow& window ) :
    mWindow( window )
{}

void System::RenderSystem::render()
{
    auto view = mRegistry->view< Component::Sprite >();

    for ( auto entity : view )
    {
        auto& sprite = view.get< Component::Sprite >( entity );
        mWindow.draw( sprite.mRectangleShape );
    }
}