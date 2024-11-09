#include "application/inc/Application.hpp"
#include "application/inc/State/MenuState.hpp"
#include "application/inc/State/GameState.hpp"
#include "application/inc/State/PauseState.hpp"

#include "core/inc/State/State.hpp"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>


const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 120.0f);

Application::Application() :
    mWindow(sf::VideoMode(640, 480), "Application Window", sf::Style::Close),
    mStateStack(Core::State::SharedObjects(mWindow)),
    mAppConsoleLogger()
{
}

void Application::initialize()
{
    LOG_INFO(mAppConsoleLogger, "Application Status: Initializing...");
    bool outDirectoryExists = false;

    // Create file path for Output Directory
    // Current Path is the Root of the Project
    std::stringstream outputDirectoryPath;

    outputDirectoryPath << std::filesystem::current_path().string();
    outputDirectoryPath << "/" << outputDirName;

    // Check to see if Directory exists
    if(std::filesystem::is_directory(outputDirectoryPath.str()))
    {
        outDirectoryExists = true;

        std::stringstream logMessage;
        logMessage << "Output Directory Already Exists ";
        logMessage << "Path:" << outputDirectoryPath.str();

        LOG_INFO(mAppConsoleLogger, logMessage.str());
    }
    else // IF NOT, create it
    {
        if( std::filesystem::create_directory(outputDirectoryPath.str()) )
        {
            outDirectoryExists = true;

            std::stringstream logMessage;
            logMessage << "SUCCESSFULLY created output directory ";
            logMessage << "Path:" << outputDirectoryPath.str();

            LOG_INFO(mAppConsoleLogger, logMessage.str());
        }
        else
        {
            std::stringstream logMessage;
            logMessage << "FAILED to created output directory";
            logMessage << "Path:" << outputDirectoryPath.str();

            LOG_ERROR(mAppConsoleLogger, logMessage.str());
            throw std::filesystem::filesystem_error("ERROR", std::error_code());
        }
    }

    // Create time stamped Folder
    if(outDirectoryExists)
    {
        // Get Time in a broken down structure
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&nowTime);

        // Get folderName
        std::stringstream folderName;
        folderName << "App_";
        folderName << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S");

        // Get entire path now
        outputDirectoryPath << "/" << folderName.str();

        // Create directory
        if( std::filesystem::create_directory(outputDirectoryPath.str()) )
        {
            std::stringstream logMessage;
            logMessage << "SUCCESSFULLY created run directory ";
            logMessage << "Path:" << outputDirectoryPath.str();

            LOG_INFO(mAppConsoleLogger, logMessage.str());
        }
        else
        {
            std::stringstream logMessage;
            logMessage << "FAILED to created run directory";
            logMessage << "Path:" << outputDirectoryPath.str();

            LOG_ERROR(mAppConsoleLogger, logMessage.str());
            throw std::filesystem::filesystem_error("ERROR", std::error_code());
        }
    }

    registerStates();
    mStateStack.pushState(States::Menu);

    LOG_INFO(mAppConsoleLogger, "Application Status: Initialized!");
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