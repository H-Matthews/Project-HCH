#pragma once

#include "application/Settings/KeyBindings.hpp"

#include "core/StateStack/StateStack.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/Messaging/MessageNetwork.hpp"
#include "core/GameAssetContainer/GameAssetContainer.hpp"

#include "utility/Logging/Logger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

#include <string>
#include <functional>

namespace Application
{
    class App
    {
      public:
        App( std::unique_ptr< Core::ConfigurationI > config );
        void initialize();
        void run();

      private:
        // Registers States in StateStack mRegistry
        void registerStates();

        void processInput();
        void update( sf::Time fixedTimeStep );
        void render();

        void initializeAppLogger();
        void initializeCoreLoggers();

      private:
        static const sf::Time TIME_PER_FRAME;

        std::shared_ptr< Utility::Logger > mAppLogger;
        // TODO: Implement a Resource Container for Textures / Fonts

        std::unique_ptr< Core::ConfigurationI > mConfiguration;
        Core::MessageNetwork mNetwork;
        Application::KeyBindings mPlayerKeyBindings;
        sf::RenderWindow mWindow;
        Core::StateStack mStateStack;
    };
}
