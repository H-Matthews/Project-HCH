#include "application/Scene/Scene.hpp"

#include "application/Scene/ECS/Entity.hpp"
#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/Systems/Systems.hpp"

#include <cmath>
#include <memory>

Application::Scene::Scene( sf::RenderWindow& window, Core::MessageNetwork& gameNetwork ) :
    mWindow( window ),
    mEntityManager()
{
    init( gameNetwork );
}

void Application::Scene::init( Core::MessageNetwork& gameNetwork )
{
    mEntityManager.registerSystem( std::make_unique< System::PlayerMovementSystem >() );
    mEntityManager.registerSystem( std::make_unique< System::PlayerInputSystem >( gameNetwork ) );

    mEntityManager.registerRenderingSystem( std::make_unique< System::RenderSystem >( mWindow ) );

    // Testing
    createEntity();

    return;
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

void Application::Scene::createEntity()
{
    Entity playerEntity = mEntityManager.createEntity();

    sf::RectangleShape rectangle( { 50.f, 50.f } );
    sf::FloatRect bounds = rectangle.getLocalBounds();
    rectangle.setOrigin( { bounds.size.x, bounds.size.y } );
    rectangle.setFillColor( sf::Color( 100, 250, 50 ) );

    sf::Vector2f initialPosition( { 320.f, 240.f } );
    sf::Vector2f initialVelocity( { 0.f, 0.f } );

    playerEntity.addComponent< Component::Sprite >( rectangle, initialPosition, initialVelocity );
    playerEntity.addComponent< Component::PlayerInput >( sf::Vector2f{ 0.f, 0.f } );
}