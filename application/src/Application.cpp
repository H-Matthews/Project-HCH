#include "Application.hpp"

#include <SFML/Graphics.hpp>


const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 120.0f);

Application::Application() :
mWindow(sf::VideoMode(640, 480), "Application Window", sf::Style::Close)
{
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while(mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);
        }

        render();
    }
}

void Application::processInput()
{
    sf::Event event;

    while(mWindow.pollEvent(event))
    {
        // TODO: Forward ALL events to PlayerInput Class

        if(event.type == sf::Event::Closed)
            mWindow.close();
    }

}

void Application::update(sf::Time fixedTimeStep)
{
    // mStateStack.update(fixedTimeStep)
}

void Application::render()
{
    mWindow.clear();
    // mStateStack.draw()
    mWindow.setView(mWindow.getDefaultView());

    mWindow.display();
}