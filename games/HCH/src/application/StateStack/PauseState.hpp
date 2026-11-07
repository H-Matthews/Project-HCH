#pragma once

#include "core/StateStack/State.hpp"

#include <string_view>

namespace Application {
class PauseState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "PAUSE";

    PauseState(Core::StateStack& stack, Core::Context ctx);

    void draw() override;

    void update(sf::Time fixedTimeStep) override;
    void handleEvent(const Core::Event& event) override;

    std::string_view getStateType() override { return TYPE; }

    // Overlay: freezes and silences the states beneath it
    bool blocksInput() const override { return true; }
    bool blocksUpdates() const override { return true; }
};
} // namespace Application
