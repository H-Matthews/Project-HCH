#include "application/StateStack/PauseState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

std::string Application::PauseState::identifier = "PauseState";

Application::PauseState::PauseState( std::string stateIdentifier ) :
    State( stateIdentifier )
{
    std::cout << "Creating PauseState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "BackSpace: Return to Main Menu" << std::endl;
    std::cout << "Escape: Return to Game " << std::endl;
}

void Application::PauseState::draw()
{
    // Draw Game related things to window here
}

bool Application::PauseState::update( sf::Time fixedTimeStep )
{
    return false;
}

bool Application::PauseState::handleKeyPressed( const sf::Event::KeyPressed& keyPressedEvent )
{
    // Pops the Pause State, returning to the GameState
    if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape )
    {
        requestStackPop();
    }
    else if ( keyPressedEvent.scancode == sf::Keyboard::Scancode::Backspace )
    {
        // Remove all states and push Menu
        requestStateClear();
        requestStackPush( "MenuState" );
    }

    return false;
}