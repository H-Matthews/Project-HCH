#include "application/inc/Application.hpp"
#include "application/inc/State/MenuState.hpp"
#include "application/inc/State/GameState.hpp"
#include "application/inc/State/PauseState.hpp"

#include "core/inc/State/State.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>


const sf::Time Application::TIME_PER_FRAME = sf::seconds(1.0f / 120.0f);

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

    // Configure Application Logger
    const std::string appOutputDir = Utility::LogRegistry::instance()->getAppOutputDir();

    // Setup TextFileSink
    auto textFileSink = std::make_shared< Utility::TextFileSink >( appOutputDir, mAppLogger->getLoggerName(), ".log" );
    textFileSink->setSinkLogLevel(Utility::LogLevel::DEBUG);

    // Setup colorConsoleSink
    auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
    colorConsoleSink->setSinkLogLevel(Utility::LogLevel::INFO);

    // Add Sinks to Logger
    Utility::Logger::sinkList list = { colorConsoleSink, textFileSink };
    mAppLogger->addSinkList(list);

    // Register App Logger
    Utility::LogRegistry::instance()->registerLogger(mAppLogger);

    // Initialize State Stack
    mStateStack.initializeLogger();

    registerStates();
    mStateStack.pushState(States::Menu);
}

void Application::registerStates()
{
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    mAppLogger->logInfo("Entering main RUN loop");
    while(mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while(timeSinceLastUpdate > TIME_PER_FRAME)
        {
            timeSinceLastUpdate -= TIME_PER_FRAME;

            processInput();
            update(TIME_PER_FRAME);

            if(mStateStack.isEmpty())
            {
                mWindow.close();
                mAppLogger->logInfo("Closing Window....");
            }
        }

        render();
    }
    mAppLogger->logInfo("Exiting main RUN loop");
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