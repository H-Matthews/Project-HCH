#include "PauseState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

PauseState::PauseState(StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects) : 
State(stack, stateIdentifier, sharedObjects)
{
    std::cout << "Creating PauseState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "BackSpace: Return to Main Menu" << std::endl;
    std::cout << "Escape: Return to Game " << std::endl;
}

void PauseState::draw()
{
    // Draw Game related things to window here

    // Retrieve window from sharedObjects struct
    sf::RenderWindow& window = *getSharedObjects().window;
}

bool PauseState::update(sf::Time fixedTimeStep)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
    if(event.type != sf::Event::KeyPressed)
        return false;

    // Return back to the game
    if(event.key.code == sf::Keyboard::Escape)
        requestStackPop();

    // Go to Main Menu
    if(event.key.code == sf::Keyboard::BackSpace)
    {
        // Remove all states and push Menu
        requestStateClear();
        requestStackPush(States::Menu);
    }

    return false;
}