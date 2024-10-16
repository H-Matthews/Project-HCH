#pragma once

#include "core/inc/State/State.hpp"

class GameState : public Core::State
{
    public:
        GameState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects);

        virtual void draw();
        virtual bool update(sf::Time fixedTimeStep);
        virtual bool handleEvent(const sf::Event& event);
};