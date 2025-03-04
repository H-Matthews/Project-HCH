#include "application/inc/State/PauseState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

Application::PauseState::PauseState(
    Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects ) :
    State( stack, stateIdentifier, sharedObjects )
{
    std::cout << "Creating PauseState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "BackSpace: Return to Main Menu" << std::endl;
    std::cout << "Escape: Return to Game " << std::endl;
}

void Application::PauseState::draw()
{
    // Draw Game related things to window here

    // Retrieve window from sharedObjects struct
    sf::RenderWindow& window = *getSharedObjects().window;
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
        requestStackPush( States::Menu );
    }

    return false;
}