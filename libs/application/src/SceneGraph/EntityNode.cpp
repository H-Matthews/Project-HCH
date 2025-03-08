#include "application/inc/SceneGraph/EntityNode.hpp"

void Application::EntityNode::setVelocity( const sf::Vector2f velocity )
{
    mVelocity = velocity;
}

void Application::EntityNode::setVelocity( const float vx, const float vy )
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Application::EntityNode::getVelocity() const
{
    return mVelocity;
}

void Application::EntityNode::accelerate( sf::Vector2f velocity )
{
    mVelocity += velocity;
}

void Application::EntityNode::accelerate( float vx, float vy )
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

void Application::EntityNode::updateCurrent( sf::Time timeStep )
{
    move( mVelocity * timeStep.asSeconds() );
}