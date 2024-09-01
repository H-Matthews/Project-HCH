#pragma once

#include "State.hpp"

#include <SFML/Graphics/Sprite.hpp>

class MenuState : public State 
{
    public:
        MenuState(StateStack& stack, SharedObjects sharedObjects);

        virtual void draw();
        virtual bool update(sf::Time fixedTimeStep);
        virtual bool handleEvent(const sf::Event& event);

    private:
        sf::Sprite mBackgroundSprite;
};