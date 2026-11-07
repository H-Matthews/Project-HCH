#include "application/StateStack/GameOverState.hpp"

#include "application/StateStack/GameState.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Event.hpp"
#include "core/GameAssetContainer/ResourceHolder.hpp"

#include <variant>

int Application::GameOverState::sWave = 0;
int Application::GameOverState::sScore = 0;

void Application::GameOverState::setResult(int wave, int score) {
    sWave = wave;
    sScore = score;
}

Application::GameOverState::GameOverState(Core::StateStack& stack, Core::Context ctx)
    : State(stack, ctx) {}

void Application::GameOverState::draw() {
    sf::RenderWindow& window = renderWindow();
    const sf::Font& font = fonts().get(Fonts::ARIAL);
    const sf::Vector2f size(window.getSize());
    const sf::Vector2f middle = size / 2.f;

    window.setView(window.getDefaultView());
    window.draw(UI::makeOverlay(size, sf::Color(0, 0, 0, 170)));

    window.draw(UI::makeText(font, "YOU DIED", 72, {middle.x, middle.y - 90.f},
                             sf::Color(220, 60, 60), true));
    window.draw(UI::makeText(
        font, "Reached wave " + std::to_string(sWave) + "   -   Score " + std::to_string(sScore),
        28, {middle.x, middle.y}, sf::Color::White, true));
    window.draw(UI::makeText(font, "Enter: play again      Esc: main menu", 22,
                             {middle.x, middle.y + 70.f}, sf::Color(190, 190, 200), true));
}

void Application::GameOverState::update(sf::Time) {
}

void Application::GameOverState::handleEvent(const Core::Event& event) {
    const auto* keyPressed = std::get_if<Core::KeyPressed>(&event);
    if (!keyPressed) {
        return;
    }

    if (keyPressed->key == Core::Key::Enter) {
        requestStateClear();
        requestStackPush<Application::GameState>();
    } else if (keyPressed->key == Core::Key::Escape) {
        requestStateClear();
        requestStackPush<Application::MenuState>();
    }

}
