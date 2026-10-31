#pragma once

#include "core/StateStack/State.hpp"

#include <string>

namespace Application {

/**
 * Shown on top of the (frozen) GameState when the player dies.
 */
class GameOverState : public Core::State {
  public:
    static std::string IDENTIFIER;

    // States are default-constructed by the StateStack, so the result is handed over statically
    static void setResult(int wave, int score);

    GameOverState();

    virtual void draw() override;
    virtual bool update(sf::Time fixedTimeStep) override;
    bool handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) override;

  private:
    static int sWave;
    static int sScore;
};
} // namespace Application
