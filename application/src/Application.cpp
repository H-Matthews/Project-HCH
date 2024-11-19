#include "application/inc/Application.hpp"
#include "application/inc/State/MenuState.hpp"
#include "application/inc/State/GameState.hpp"
#include "application/inc/State/PauseState.hpp"

#include "core/inc/State/State.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>


const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 120.0f);

Application::Application(std::shared_ptr<Core::ConfigurationI> config) :
    mWindow(sf::VideoMode(640, 480), "Application Window", sf::Style::Close),
    mStateStack(Core::State::SharedObjects(mWindow)),
    mConfiguration(config),
    mAppConsoleLogger(std::make_unique<Utility::ConsoleLogger>()),
    mAppTextLogger(nullptr)
{
}

void Application::initialize()
{
    // Configure Application
    mConfiguration->initializeIteration();
    mConfiguration->loadSettings();

    // Initialize TextFile Logger
    Utility::LogRegistry::getInstance()->registerLogger("Application");
    mAppTextLogger = Utility::LogRegistry::getInstance()->getLogger("Application");

    // Initialize State Stack
    registerStates();
    mStateStack.pushState(States::Menu);

    LOG_INFO(mAppTextLogger, "Application Initialized " );
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

            if(mStateStack.isEmpty())
                mWindow.close();
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
        mStateStack.handleEvent(event);

        if(event.type == sf::Event::Closed)
            mWindow.close();
    }

}

void Application::update(sf::Time fixedTimeStep)
{
    mStateStack.update(fixedTimeStep);
}

void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Application::registerStates()
{
    mStateStack.registerState<MenuState>(States::Menu);
    LOG_INFO(mAppTextLogger, "Registered Menu State");

    mStateStack.registerState<GameState>(States::Game);
    LOG_INFO(mAppTextLogger, "Registered Game State");

    mStateStack.registerState<PauseState>(States::Pause);
    LOG_INFO(mAppTextLogger, "Registered Pause State");
}