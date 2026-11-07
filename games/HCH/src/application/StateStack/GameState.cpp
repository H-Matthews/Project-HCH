#include "application/StateStack/GameState.hpp"

#include "application/StateStack/GameOverState.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/PauseState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/GameAssetContainer/ResourceHolder.hpp"
#include "core/InputState.hpp"

#include <string>

Application::GameTuning Application::GameState::sTuning;

void Application::GameState::setTuning(const GameTuning& tuning) {
    sTuning = tuning;
}

Application::GameState::GameState(Core::StateStack& stack, Core::Context ctx) : State(stack, ctx) {}

bool Application::GameState::initializeState() {
    mScene = std::make_unique<Scene>(renderWindow(), sTuning);

    return true;
}

void Application::GameState::handleEvent(const Core::Event& event) {
    if (const auto* focusLost = std::get_if<Core::FocusLost>(&event)) {
        requestStackPush<Application::PauseState>();
    }
}

void Application::GameState::update(sf::Time fixedTimeStep) {
    // Held keys (movement) act every tick; one-shot actions (attack) act on the tick of the press
    for (const auto& [key, action] : mKeyBindings.getKeyBindings()) {
        const bool triggered =
            KeyBindings::isRealTimeAction(action) ? input().isDown(key) : input().wasPressed(key);
        if (triggered) {
            mScene->applyAction(action);
        }
    }

    if (input().wasPressed(Core::Key::P) || input().wasPressed(Core::Key::Escape)) {
        requestStackPush<Application::PauseState>();
    } else if (input().wasPressed(Core::Key::Delete)) {
        requestStackPop();
        requestStackPush<Application::MenuState>();
    }

    mScene->update(fixedTimeStep);

    if (mScene->stats().playerDead && !mGameOverRequested) {
        mGameOverRequested = true;
        GameOverState::setResult(mScene->stats().wave, mScene->stats().score);
        requestStackPush<GameOverState>();
    }
}

void Application::GameState::draw() {
    mScene->draw();
    drawHud();
}

void Application::GameState::drawHud() {
    sf::RenderWindow& window = renderWindow();
    const sf::Font& font = fonts().get(Fonts::ARIAL);

    window.setView(window.getDefaultView());

    // Health bar
    constexpr float BAR_WIDTH = 260.f;
    const float maxHealth = mScene->playerMaxHealth();
    const float fraction = maxHealth > 0.f ? mScene->playerHealth() / maxHealth : 0.f;

    sf::RectangleShape bar({BAR_WIDTH, 22.f});
    bar.setPosition({20.f, 20.f});
    bar.setFillColor(sf::Color(60, 20, 20));
    bar.setOutlineColor(sf::Color(200, 200, 200));
    bar.setOutlineThickness(2.f);
    window.draw(bar);

    bar.setSize({BAR_WIDTH * fraction, 22.f});
    bar.setFillColor(sf::Color(200, 50, 50));
    bar.setOutlineThickness(0.f);
    window.draw(bar);

    const auto& stats = mScene->stats();
    window.draw(UI::makeText(
        font, "Wave " + std::to_string(stats.wave) + "    Score " + std::to_string(stats.score), 22,
        {20.f, 52.f}));
    window.draw(UI::makeText(
        font, "WASD/Arrows: move   Space/J: attack   P/Escape: pause, Del: menu", 16,
        {20.f, static_cast<float>(window.getSize().y) - 32.f}, sf::Color(170, 170, 180)));
}
