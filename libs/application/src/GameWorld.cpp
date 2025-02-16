#include "application/inc/GameWorld.hpp"


Application::GameWorld::GameWorld(sf::RenderWindow& window) :
    mWindow(window),
    mSceneGraph(),
    mSceneLayers()
{
}

void Application::GameWorld::update(sf::Time fixedTimeStep)
{

}

void Application::GameWorld::draw()
{
    mWindow.draw(mSceneGraph);
}