#pragma once

#include "application/Settings/KeyBindings.hpp"

#include "core/StateStack/StateStack.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/Messaging/MessageNetwork.hpp"
#include "core/GameAssetContainer/GameAssetContainer.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/GameAssetContainer/ResourceEnums.hpp"

#include "utility/Logging/Logger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include <string>
#include <functional>

namespace Application
{
    enum class State
    {
        NONE = 0, // This IS NOT a valid value, just used for default values
        NOT_CONFIGURED,
        CONFIGURED,
        WAITING_TO_RUN,
        RUNNING,
        SHUTTING_DOWN
    };

    std::string convertEnumToString( const State& state );
    State convertStringToEnum( const std::string& stringState );

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

        void loadResources();

        bool transitionState( State statusToTransfer );

        inline bool isConfigured()
        {
            return mState == State::CONFIGURED;
        }

        inline bool isWaitingToRun()
        {
            return mState == State::WAITING_TO_RUN;
        }

        inline bool isRunning()
        {
            return mState == State::RUNNING;
        }

      private:
        static const sf::Time TIME_PER_FRAME;

        State mState;

        std::shared_ptr< Utility::Logger > mAppLogger;

        // Should this go here? or in the GameState?
        TextureHolder mTextures;

        std::unique_ptr< Core::ConfigurationI > mConfiguration;
        Core::MessageNetwork mNetwork;
        Application::KeyBindings mPlayerKeyBindings;
        sf::RenderWindow mWindow;
        Core::StateStack mStateStack;
    };
}
