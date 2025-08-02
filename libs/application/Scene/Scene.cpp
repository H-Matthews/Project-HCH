#include "application/Scene/Scene.hpp"

#include "application/Scene/ECS/Entity.hpp"
#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/Systems/Systems.hpp"

#include <cmath>
#include <memory>

Application::Scene::Scene( sf::RenderWindow& window, Core::MessageNetwork& gameNetwork, TextureHolder& textures ) :
    mWindow( window ),
    mEntityManager( textures )
{
    initializeGameSystems( gameNetwork );

    initializePlayerEntity();
}

void Application::Scene::initializeGameSystems( Core::MessageNetwork& gameNetwork )
{
    mEntityManager.registerSystem( std::make_unique< System::PlayerMovementSystem >() );
    mEntityManager.registerSystem( std::make_unique< System::PlayerInputSystem >( gameNetwork ) );

    mEntityManager.registerRenderingSystem( std::make_unique< System::RenderSystem >( mWindow ) );

    return;
}

void Application::Scene::initializePlayerEntity()
{
    // TODO: Should be relative to the window
    sf::Vector2f initialPosition( { 320.f, 240.f } );
    sf::Vector2f initialVelocity( { 0.f, 0.f } );

    mEntityManager.createPlayerEntity( initialPosition, initialVelocity );
}

void Application::Scene::update( sf::Time fixedTimeStep )
{
    mEntityManager.update( fixedTimeStep );

    return;
}

void Application::Scene::draw()
{
    mEntityManager.render();

    return;
}