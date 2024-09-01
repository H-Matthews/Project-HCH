#include "MenuState.hpp"

#include <iostream>

MenuState::MenuState(StateStack& stack, SharedObjects sharedObjects) :
State(stack, sharedObjects)
{
    std::cout << "Creating MenuState " << std::endl;
    
    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "G: Print message" << std::endl;
    std::cout << "Enter: Transitions to GameState" << std::endl;
    std::cout << "Escape: Closes Program " << std::endl;
}

void MenuState::draw()
{
    // Draw Menu related things to window here

    // Retrieve window from sharedObjects struct
    sf::RenderWindow& window = *getSharedObjects().window;

    window.setView(window.getDefaultView());
    //window.draw(mBackgroundSprite);
}

bool MenuState::update(sf::Time fixedTimeStep)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    if( event.type != sf::Event::KeyPressed)
        return false;

    if(event.key.code == sf::Keyboard::G)
    {
        std::cout << "Handling Events in MenuState. You Pressed the G key " << std::endl;
    }

    else if(event.key.code == sf::Keyboard::Return)
    {
        requestStackPop();
        requestStackPush(States::Game);
    }

    else if(event.key.code == sf::Keyboard::Escape)
    {
        requestStackPop();
    }

    return true;
}