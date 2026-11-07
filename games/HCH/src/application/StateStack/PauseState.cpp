#include "application/StateStack/PauseState.hpp"

#include "application/StateStack/MenuState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Event.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"

#include <variant>


Application::PauseState::PauseState(Core::StateStack& stack, Core::Context ctx)
    : State(stack, ctx) {}

void Application::PauseState::draw() {
    sf::RenderWindow& window = renderWindow();
    const sf::Font& font = fonts().get(Fonts::ARIAL);
    const sf::Vector2f size(window.getSize());
    const sf::Vector2f middle = size / 2.f;

    window.setView(window.getDefaultView());
    window.draw(UI::makeOverlay(size, sf::Color(0, 0, 0, 150)));

    window.draw(
        UI::makeText(font, "PAUSED", 64, {middle.x, middle.y - 50.f}, sf::Color::White, true));
    window.draw(UI::makeText(font, "P / Esc: resume      Backspace: main menu", 22,
                             {middle.x, middle.y + 30.f}, sf::Color(190, 190, 200), true));
}

// Returning false stops the GameState underneath from updating
bool Application::PauseState::update(sf::Time) {
    return false;
}

bool Application::PauseState::handleEvent(const Core::Event& event) {
    const auto* keyPressed = std::get_if<Core::KeyPressed>(&event);
    if (!keyPressed) {
        return false;
    }

    // Pops the Pause State, returning to the GameState
    if (keyPressed->key == Core::Key::Escape ||
        keyPressed->key == Core::Key::P) {
        requestStackPop();
    } else if (keyPressed->key == Core::Key::Backspace) {
        // Remove all states and push Menu
        requestStateClear();
        requestStackPush<Application::MenuState>();
    }

    return false;
}
