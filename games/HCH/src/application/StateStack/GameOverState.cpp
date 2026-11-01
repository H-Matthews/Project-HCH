#include "application/StateStack/GameOverState.hpp"

#include "application/StateStack/GameState.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Application.hpp"

std::string Application::GameOverState::IDENTIFIER = "GAME_OVER";
int Application::GameOverState::sWave = 0;
int Application::GameOverState::sScore = 0;

void Application::GameOverState::setResult(int wave, int score) {
    sWave = wave;
    sScore = score;
}

Application::GameOverState::GameOverState() : State(IDENTIFIER) {}

void Application::GameOverState::draw() {
    sf::RenderWindow& window = getApplication().getRenderWindow();
    const sf::Font& font = getApplication().getFonts().get(Fonts::ARIAL);
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

// Returning false freezes the GameState underneath
bool Application::GameOverState::update(sf::Time) {
    return false;
}

bool Application::GameOverState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) {
    if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Enter) {
        requestStateClear();
        requestStackPush<Application::GameState>();
    } else if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape) {
        requestStateClear();
        requestStackPush<Application::MenuState>();
    }

    return false;
}
