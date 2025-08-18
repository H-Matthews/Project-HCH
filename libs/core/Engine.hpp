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
    enum class EngineState
    {
        NONE = 0, // This IS NOT a valid value, just used for default values
        WAITING_TO_RUN,
        RUNNING,
        SHUTTING_DOWN
    };

    std::string convertEngineStateEnumToString( const EngineState& state );
    EngineState convertStringToEngineStateEnum( const std::string& stringState );

    class Engine : public Core::Configurable
    {
      public:
        static const std::string TYPE_NAME;

        Engine();
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

        bool transitionState( EngineState statusToTransfer );

        inline bool isWaitingToRun() const;
        inline bool isRunning() const;

      private:
        static const sf::Time TIME_PER_FRAME;

        EngineState mState;

        std::shared_ptr< Utility::Logger > mEngineLogger;

        // Should this go here? or in the GameState?
        TextureHolder mTextures;

        std::unique_ptr< Core::Configuration > mConfiguration;
        Core::MessageNetwork mNetwork;
        // Application::KeyBindings mPlayerKeyBindings;
        sf::RenderWindow mWindow;
        Core::StateStack mStateStack;
    };

    bool Engine::isWaitingToRun() const
    {
        return mState == EngineState::WAITING_TO_RUN;
    }

    bool Engine::isRunning() const
    {
        return mState == EngineState::RUNNING;
    }

    void Core::Engine::setConfiguration( std::unique_ptr< Core::Configuration > config )
    {
        if ( mEngineLogger )
            mEngineLogger->logInfo( "Hello" );
        mConfiguration = std::move( config );
    }
}
