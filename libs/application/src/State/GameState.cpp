#include "application/inc/State/GameState.hpp"

#include <iostream>

Application::GameState::GameState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects) : 
    State(stack, stateIdentifier, sharedObjects),
    mNetwork(*sharedObjects.network),
    mGameWorld(*sharedObjects.window),
    mKeyBindings(),
    mPlayerPublisher(sharedObjects.network, mKeyBindings)
{



    std::cout << "Creating GameState " << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "Enter: Print message" << std::endl;
    std::cout << "P: Pause Game " << std::endl;
    std::cout << "Escape: Return To Menu State " << std::endl;
}

void Application::GameState::draw()
{
    mGameWorld.draw();
    // This will be just mWorld.draw() later
}

bool Application::GameState::update(sf::Time fixedTimeStep)
{
    // Disseminate Messages
    mNetwork.notifySubscribers();

    mGameWorld.update(fixedTimeStep);

    return true;
}

bool Application::GameState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent)
{
    // Game Input Handling
    mPlayerPublisher.handleKeyPressed(keyPressedEvent);

    if(keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter)
    {
        std::cout << "Handling Events in GameState. You pressed the enter key " << std::endl;
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