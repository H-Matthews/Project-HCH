#include "application/StateStack/GameState.hpp"

#include "application/StateStack/GameOverState.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/PauseState.hpp"
#include "application/Data/GameResources.hpp"
#include "application/UI/TextUtil.hpp"

#include "core/Application.hpp"
#include "core/StateStack/StateStack.hpp"

#include <string>

std::string Application::GameState::IDENTIFIER = "GAME";

Application::GameState::GameState() : State(IDENTIFIER) {}

// Used to set references to external subsystems
bool Application::GameState::initializeState() {
    if (!mStackRef)
        return false;

    Core::Application& app = getApplication();
    Core::MessageNetwork& network = *app.getNetwork();

    const GameTuning tuning =
        GameTuning::fromConfig(app.getConfigSection(GameTuning::SECTION_NAME));
    mScene = std::make_unique<Scene>(app.getWindow(), network, tuning);
    mPlayerInputPublisher = std::make_unique<PlayerInputPublisher>(network, mKeyBindings);

    return true;
}

void Application::GameState::draw() {
    mScene->draw();
    drawHud();
}

void Application::GameState::drawHud() {
    sf::RenderWindow& window = getApplication().getWindow();
    const sf::Font& font = getApplication().getFonts().get(Fonts::ARIAL);

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
    window.draw(UI::makeText(font, "WASD/Arrows: move   Space/J: attack   P: pause", 16,
                             {20.f, static_cast<float>(window.getSize().y) - 32.f},
                             sf::Color(170, 170, 180)));
}

bool Application::GameState::update(sf::Time fixedTimeStep) {
    mScene->update(fixedTimeStep);

    if (mScene->stats().playerDead && !mGameOverRequested) {
        mGameOverRequested = true;
        GameOverState::setResult(mScene->stats().wave, mScene->stats().score);
        requestStackPush<GameOverState>();
    }

    return true;
}

bool Application::GameState::handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) {
    // Handle Event based Key Presses
    mPlayerInputPublisher->handleKeyPressed(keyPressedEvent);

    if (keyPressedEvent.scancode == sf::Keyboard::Scancode::P) {
        requestStackPush<Application::PauseState>();
    } else if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape) {
        requestStackPop();
        requestStackPush<Application::MenuState>();
    }

    return true;
}

bool Application::GameState::handleRealTimeInput() {
    // Handle RealTime Input KeyPresses (movement)
    mPlayerInputPublisher->handleRealTimeInput();

    return true;
}

Application::GameState::~GameState() = default;
