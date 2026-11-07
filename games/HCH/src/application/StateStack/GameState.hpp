#pragma once

#include "application/Scene/Scene.hpp"
#include "application/Scene/ECS/GameTuning.hpp"
#include "application/Settings/KeyBindings.hpp"

#include "core/StateStack/State.hpp"

#include <memory>
#include <string_view>

namespace Application {

/**
 * High Level class that contains everything the game would need to run when playing the game
 */
class GameState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "GAME";

    // States are constructed by the StateStack, so the tuning is handed over statically
    static void setTuning(const GameTuning& tuning);

    GameState(Core::StateStack& stack, Core::Context ctx);
    ~GameState() override = default;

    bool initializeState() override;

    void handleEvent(const Core::Event& event) override;
    void update(sf::Time fixedTimeStep) override;
    void draw() override;

    std::string_view getStateType() override { return TYPE; }

  private:
    void drawHud();

  private:
    static GameTuning sTuning;

    Application::KeyBindings mKeyBindings;
    std::unique_ptr<Scene> mScene;

    bool mGameOverRequested = false;
};
} // namespace Application
