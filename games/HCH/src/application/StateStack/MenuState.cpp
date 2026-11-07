#include "application/StateStack/MenuState.hpp"

#include "application/StateStack/GameState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Event.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"

#include <variant>


Application::MenuState::MenuState(Core::StateStack& stack, Core::Context ctx)
    : State(stack, ctx) {}

void Application::MenuState::handleEvent(const Core::Event& event) {
    const auto* keyPressed = std::get_if<Core::KeyPressed>(&event);
    if (!keyPressed) {
        return;
    }

    if (keyPressed->key == Core::Key::Enter) {
        requestStackPop();

        // Push GameState
        requestStackPush<Application::GameState>();
    } else if (keyPressed->key == Core::Key::Escape) {
        requestStackPop();
    }

}

void Application::MenuState::update(sf::Time) {
}

void Application::MenuState::draw() {
    sf::RenderWindow& window = renderWindow();
    const sf::Font& font = fonts().get(Fonts::ARIAL);
    const sf::Vector2f middle = sf::Vector2f(window.getSize()) / 2.f;

    window.setView(window.getDefaultView());

    window.draw(UI::makeText(font, "PROJECT HCH", 84, {middle.x, middle.y - 120.f},
                             sf::Color(240, 200, 90), true));
    window.draw(UI::makeText(font, "Survive the waves", 28, {middle.x, middle.y - 50.f},
                             sf::Color(190, 190, 200), true));
    window.draw(UI::makeText(font, "Enter: Start        Esc: Quit", 26, {middle.x, middle.y + 40.f},
                             sf::Color::White, true));
    window.draw(UI::makeText(font, "WASD / Arrows: move     Space / J: attack     P/Escape: pause    Del: main menu", 18,
                             {middle.x, middle.y + 110.f}, sf::Color(150, 150, 165), true));
}