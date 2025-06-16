#pragma once

#include "application/Scene/Scene.hpp"
#include "application/Messaging/Publishers/PlayerInputPublisher.hpp"

#include "core/StateStack/State.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/GameAssetContainer/ResourceEnums.hpp"

namespace Application
{

    /**
     * High Level class that contains everything the game would need to run when playing the game
     */
    class GameState : public Core::State
    {
      public:
        GameState( Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects );

        virtual void draw();
        virtual bool update( sf::Time fixedTimeStep );
        bool handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent ) override;
        bool handleRealTimeInput() override;

        ~GameState();

      private:
        Core::MessageNetwork& mGameNetwork;

        Application::Scene mScene;
        Application::KeyBindings mKeyBindings;
        Application::PlayerInputPublisher mPlayerInputPublisher;
    };
}