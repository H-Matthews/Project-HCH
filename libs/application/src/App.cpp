#include "application/inc/App.hpp"
#include "application/inc/State/MenuState.hpp"
#include "application/inc/State/GameState.hpp"
#include "application/inc/State/PauseState.hpp"

#include "core/inc/State/State.hpp"

#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>


const sf::Time Application::App::TIME_PER_FRAME = sf::seconds(1.0f / 120.0f);

Application::App::App(std::shared_ptr<Core::ConfigurationI> config) :
    mAppLogger(std::make_shared<Utility::Logger>("AppLogger")),
    mWindow(sf::VideoMode(640, 480), "App Window", sf::Style::Close),
    mConfiguration(config),
    mStateStack(Core::State::SharedObjects(mWindow, mNetwork)),
    mNetwork(),
    mPlayerKeyBindings()
{
}

void Application::App::initialize()
{
    mConfiguration->initializeIteration();

    if constexpr (Utility::CAN_LOG)
    {
        initializeAppLogger();
        initializeCoreLoggers();
    }

    // Initialize State Stack
    registerStates();
    mStateStack.pushState(States::Menu);
}

void Application::App::registerStates()
{
    mStateStack.registerState<Application::MenuState>(States::Menu);
    mStateStack.registerState<Application::GameState>(States::Game);
    mStateStack.registerState<Application::PauseState>(States::Pause);
}

void Application::App::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    if constexpr (Utility::CAN_LOG)
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

                if constexpr (Utility::CAN_LOG)
                    mAppLogger->logInfo("Closing Window....");
            }
        }
        render();
    }

    if constexpr (Utility::CAN_LOG)
        mAppLogger->logInfo("Exiting main RUN loop");
}

void Application::App::processInput()
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

void Application::App::update(sf::Time fixedTimeStep)
{
    mStateStack.update(fixedTimeStep);
}

void Application::App::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}

void Application::App::initializeAppLogger()
{
    // Configure App Logger
    const std::string appOutputDir = Utility::LogRegistry::instance()->getAppOutputDir();

    // Setup Sinks
    auto textFileSink = std::make_shared< Utility::TextFileSink >( appOutputDir, 
                                                                   "App",
                                                                   ".log",
                                                                   Utility::LogLevel::DEBUG);

    auto colorConsoleSink = std::make_shared< Utility::ColorConsoleSink >(Utility::LogLevel::INFO);

    // Add Sinks to Logger
    Utility::Logger::sinkList list = { colorConsoleSink, textFileSink };
    mAppLogger->addSinkList(list);

    // Register App Logger
    Utility::LogRegistry::instance()->registerLogger(mAppLogger);
}

void Application::App::initializeCoreLoggers()
{
    mStateStack.initializeLogger();
    mNetwork.initializeLogger();
}