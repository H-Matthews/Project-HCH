#include "application/inc/Entity.hpp"


void Application::Entity::setVelocity(const sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Application::Entity::setVelocity(const float vx, const float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Application::Entity::getVelocity() const
{
    return mVelocity;
}

void Application::Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
}

void Application::Entity::accelerate(float vx, float vy)
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

void Application::Entity::updateCurrent(sf::Time timeStep)
{
    move(mVelocity * timeStep.asSeconds());
}