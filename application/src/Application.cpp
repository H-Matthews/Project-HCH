#include "application/inc/Application.hpp"
#include "application/inc/State/MenuState.hpp"
#include "application/inc/State/GameState.hpp"
#include "application/inc/State/PauseState.hpp"

#include "core/inc/State/State.hpp"

// Logging
#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>


const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 120.0f);

Application::Application(std::shared_ptr<Core::ConfigurationI> config) :
    mAppLogger(std::make_shared<Utility::Logger>("Application")),
    mWindow(sf::VideoMode(640, 480), "Application Window", sf::Style::Close),
    mStateStack(Core::State::SharedObjects(mWindow, mAppLogger->getLoggerName())),
    mConfiguration(config)
{
}

void Application::initialize()
{
    // Configure Application
    mConfiguration->initializeIteration();
    mConfiguration->loadSettings();

    // Configure Logging
    // Configure Application Logger
    const std::string appOutputDir = Utility::LogRegistry::instance()->getAppOutputDir();
    std::shared_ptr< Utility::TextFileSink > textFileSink = nullptr;
    if(appOutputDir != "")
    {
        textFileSink = std::make_shared< Utility::TextFileSink >( appOutputDir, mAppLogger->getLoggerName(), ".log");
        textFileSink->setSinkLogLevel(Utility::LogLevel::DEBUG);
    }

    auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
    colorConsoleSink->setSinkLogLevel(Utility::LogLevel::INFO);

    Utility::Logger::sinkList list = { colorConsoleSink, textFileSink };
    mAppLogger->addSinkList(list);

    // Finally, register logger with the registry so we can get it from anywhere
    Utility::LogRegistry::instance()->registerLogger(mAppLogger);

    // Initialize State Stack
    registerStates();
    mStateStack.pushState(States::Menu);
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
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
}