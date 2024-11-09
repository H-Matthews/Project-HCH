#pragma once

#include "core/inc/State/StateStack.hpp"
#include "utility/inc/ConsoleLogger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class Application
{
    public:
        Application();
        void initialize();
        void run();

    private:

        void processInput();
        void update(sf::Time fixedTimeStep);
        void render();

        // Registers States in StateStack mRegistry
        void registerStates();

    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow mWindow;
        // TODO: Implement a Resource Container for Textures / Fonts

        Core::StateStack mStateStack;

        // Related to Logging
        Utility::ConsoleLogger mAppConsoleLogger;
        const std::string outputDirName = "output";
};