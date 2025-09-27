#pragma once

#include "core/StateStack/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace Application
{
    class MenuState : public Core::State
    {
      public:
        static std::string identifier;

        MenuState( std::string stateName );
        MenuState( Core::StateStack& stack, std::string stateIdentifier );

        virtual void draw();
        virtual bool update( sf::Time fixedTimeStep );
        bool handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent ) override;
        bool handleMouseMoved( const sf::Event::MouseMoved& mouseMovedEvent ) override;
    };
}