#include "application/inc/State/MenuState.hpp"

#include <iostream>

Application::MenuState::MenuState(Core::StateStack& stack, std::string stateIdentifier, SharedObjects sharedObjects) :
    State(stack, stateIdentifier, sharedObjects)
{
    std::cout << "Creating MenuState " << std::endl;
    
    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "G: Print message" << std::endl;
    std::cout << "Enter: Transitions to GameState" << std::endl;
    std::cout << "Escape: Closes Program " << std::endl;
}

void Application::MenuState::draw()
{
    // Draw Menu related things to window here

    // Retrieve window from sharedObjects struct
    sf::RenderWindow& window = *getSharedObjects().window;

    window.setView(window.getDefaultView());
    //window.draw(mBackgroundSprite);
}

bool Application::MenuState::update(sf::Time fixedTimeStep)
{
    return true;
}

bool Application::MenuState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent)
{
    if(keyPressedEvent.scancode == sf::Keyboard::Scancode::G)
    {
        std::cout << "Handling Events in Menustate. You Pressed the G Key " << std::endl;
    }
    else if(keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter)
    {
        requestStackPop();
        requestStackPush(States::Game);
    }
    else if(keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape)
    {
        requestStackPop();
    }

    return true;
}

bool Application::MenuState::handleMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent)
{
    std::cout << "Moved the mouse in Menu State " << std::endl;

    return true;
}