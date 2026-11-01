#include "application/StateStack/MenuState.hpp"

#include "application/StateStack/GameState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Application.hpp"

std::string Application::MenuState::IDENTIFIER = "MENU";

Application::MenuState::MenuState() : State(IDENTIFIER) {}

void Application::MenuState::draw() {
    sf::RenderWindow& window = getApplication().getRenderWindow();
    const sf::Font& font = getApplication().getFonts().get(Fonts::ARIAL);
    const sf::Vector2f middle = sf::Vector2f(window.getSize()) / 2.f;

    window.setView(window.getDefaultView());

    window.draw(UI::makeText(font, "PROJECT HCH", 84, {middle.x, middle.y - 120.f},
                             sf::Color(240, 200, 90), true));
    window.draw(UI::makeText(font, "Survive the waves", 28, {middle.x, middle.y - 50.f},
                             sf::Color(190, 190, 200), true));
    window.draw(UI::makeText(font, "Enter: Start        Esc: Quit", 26, {middle.x, middle.y + 40.f},
                             sf::Color::White, true));
    window.draw(UI::makeText(font, "WASD / Arrows: move     Space / J: attack     P: pause", 18,
                             {middle.x, middle.y + 110.f}, sf::Color(150, 150, 165), true));
}

bool Application::MenuState::update(sf::Time) {
    return true;
}

bool Application::MenuState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) {
    if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter) {
        requestStackPop();

        // Push GameState
        requestStackPush<Application::GameState>();
    } else if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape) {
        requestStackPop();
    }

    return true;
}
