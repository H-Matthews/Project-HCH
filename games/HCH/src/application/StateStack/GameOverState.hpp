#pragma once

#include "core/StateStack/State.hpp"

#include <string_view>

namespace Application {

/**
 * Shown on top of the (frozen) GameState when the player dies.
 */
class GameOverState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "GAME_OVER";

    // States are default-constructed by the StateStack, so the result is handed over statically
    static void setResult(int wave, int score);

    GameOverState(Core::StateStack& stack, Core::Context ctx);

    virtual void draw() override;
    virtual bool update(sf::Time fixedTimeStep) override;
    bool handleEvent(const Core::Event& event) override;

    std::string_view getStateType() override { return TYPE; }

  private:
    static int sWave;
    static int sScore;
};
} // namespace Application
