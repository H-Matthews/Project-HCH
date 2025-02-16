#pragma once

#include "application/inc/GameWorld.hpp"
#include "application/inc/MessageNetwork/Publishers/PlayerInputPublisher.hpp"

#include "core/inc/State/State.hpp"

namespace Application
{
    class GameState : public Core::State
    {
        public:
            GameState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects);

            virtual void draw();
            virtual bool update(sf::Time fixedTimeStep);
            bool handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) override;

        private:
            Core::MessageNetwork& mNetwork;

            Application::GameWorld mGameWorld;
            Application::KeyBindings mKeyBindings;
            Application::PlayerInputPublisher mPlayerPublisher;

    };
}