#pragma once

#include "application/Settings/KeyBindings.hpp"

#include "core/Configuration/Configurables/Configurable.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/StateStack/StateStack.hpp"
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

namespace Core
{
    class Application : public Core::Configurable
    {
      public:
        enum class State
        {
            NONE = 0, // This IS NOT a valid value, just used for default values
            WAITING_TO_RUN,
            RUNNING,
            SHUTTING_DOWN
        };

        std::string convertEngineStateEnumToString( const State& state );
        State convertStringToEngineStateEnum( const std::string& stringState );

      public:
        static const std::string TYPE_NAME;

        Application( Core::ConfigSpec ConfigSpec );
        inline void setConfiguration( std::unique_ptr< Core::Configuration > config );
        void initialize();
        void run();

      private:
        // Registers States in StateStack
        void registerStates();

        void processInput();
        void update( sf::Time fixedTimeStep );
        void render();

        void loadResources();

        bool transitionState( State statusToTransfer );

        inline bool isWaitingToRun() const;
        inline bool isRunning() const;

      private:
        static const sf::Time TIME_PER_FRAME;

        std::unique_ptr< Core::Configuration > mConfiguration;

        State mState;

        // Should this go here? or in the GameState?
        TextureHolder mTextures;

        // CORE APPLICATION CLASSES
        MessageNetwork mNetwork;
        StateStack mStateStack;

        // TODO: These need to go into a CORE Class
        // Application::KeyBindings mPlayerKeyBindings;
        sf::RenderWindow mWindow;
    };

    bool Application::isWaitingToRun() const
    {
        return mState == State::WAITING_TO_RUN;
    }

    bool Application::isRunning() const
    {
        return mState == State::RUNNING;
    }

    void Core::Application::setConfiguration( std::unique_ptr< Core::Configuration > config )
    {
        mConfiguration = std::move( config );
    }
}
