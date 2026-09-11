#include "application/StateStack/MenuState.hpp"

#include "application/StateStack/GameState.hpp"

#include <iostream>

std::string Application::MenuState::IDENTIFIER = "MENU";

Application::MenuState::MenuState() : State(IDENTIFIER) {
    std::cout << "Creating " << getStateName() << " State" << std::endl;

    std::cout << "Controls: --------------------------------" << std::endl;
    std::cout << "G: Print message" << std::endl;
    std::cout << "Enter: Transitions to GameState" << std::endl;
    std::cout << "Escape: Closes Program " << std::endl;
}

void Application::MenuState::draw() {
    // Draw Menu related things to window here

    // window.setView( window.getDefaultView() );
    // window.draw(mBackgroundSprite);
}

bool Application::MenuState::update(sf::Time fixedTimeStep) {
    return true;
}

bool Application::MenuState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) {
    if (keyPressedEvent.scancode == sf::Keyboard::Scancode::G) {
        std::cout << "Handling Events in Menustate. You Pressed the G Key " << std::endl;
    } else if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter) {
        requestStackPop();

        // Push GameState
        requestStackPush<Application::GameState>();
    } else if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape) {
        requestStackPop();
    }

    return true;
}

bool Application::MenuState::handleMouseMoved(const sf::Event::MouseMoved& mouseMovedEvent) {
    return true;
}