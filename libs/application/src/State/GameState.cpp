#include "application/inc/State/GameState.hpp"

#include <iostream>

Application::GameState::GameState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects) : 
    State(stack, stateIdentifier, sharedObjects)
{
    std::cout << "Creating GameState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "Enter: Print message" << std::endl;
    std::cout << "P: Pause Game " << std::endl;
    std::cout << "Escape: Return To Menu State " << std::endl;
}

void Application::GameState::draw()
{
    // Draw Game related things to window here

    // Retrieve window from sharedObjects struct
    sf::RenderWindow& window = *getSharedObjects().window;

    // This will be just mWorld.draw() later
}

bool Application::GameState::update(sf::Time fixedTimeStep)
{
    return true;
}

bool Application::GameState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent)
{

    if(keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter)
    {
        std::cout << "Handling Events in GameState. You prseed the enter key " << std::endl;
    }
    else if(keyPressedEvent.scancode == sf::Keyboard::Scancode::P)
    {
        requestStackPush(States::Pause);
    }
    else if(keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
    
    return true;
}