#pragma once

#include "State.hpp"

class GameState : public State
{
    public:
        GameState(StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects);

        virtual void draw();
        virtual bool update(sf::Time fixedTimeStep);
        virtual bool handleEvent(const sf::Event& event);
};