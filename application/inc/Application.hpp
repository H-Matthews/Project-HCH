#pragma once

#include "core/inc/State/StateStack.hpp"
#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/Logger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <string>

class Application
{
    public:
        Application(std::shared_ptr<Core::ConfigurationI> config);
        void initialize();
        void run();

    private:
        // Registers States in StateStack mRegistry
        void registerStates();

        void processInput();
        void update(sf::Time fixedTimeStep);
        void render();


    private:
        static const sf::Time TimePerFrame;

        std::shared_ptr< Utility::Logger > mAppLogger;
        sf::RenderWindow mWindow;
        // TODO: Implement a Resource Container for Textures / Fonts

        Core::StateStack mStateStack;
        std::shared_ptr<Core::ConfigurationI> mConfiguration;

};