#pragma once

#include "core/StateStack/State.hpp"

#include <string_view>

namespace Application {
class MenuState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "MENU";

    MenuState(Core::StateStack& stack, Core::Context ctx);

    void handleEvent(const Core::Event& event) override;
    void update(sf::Time fixedTimeStep) override;
    void draw() override;

    std::string_view getStateType() override { return TYPE; }
};
} // namespace Application
