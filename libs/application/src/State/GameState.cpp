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

bool Application::GameState::handleEvent(const sf::Event& event)
{
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
    {
        std::cout << "Handling Events in GameState. You Pressed the Enter key " << std::endl;
    }

    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
    {
        requestStackPush(States::Pause);
    }

    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
    
    return true;
}