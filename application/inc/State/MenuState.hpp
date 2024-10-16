#pragma once

#include "core/inc/State/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

class MenuState : public Core::State 
{
    public:
        MenuState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects);

        virtual void draw();
        virtual bool update(sf::Time fixedTimeStep);
        virtual bool handleEvent(const sf::Event& event);

    private:
        sf::Sprite mBackgroundSprite;
};