#pragma once

#include "core/StateStack/State.hpp"

#include <string_view>

namespace Application {
class MenuState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "MENU";

    MenuState(Core::StateStack& stack, Core::Context ctx);

    virtual void draw() override;
    virtual bool update(sf::Time fixedTimeStep) override;
    bool handleEvent(const Core::Event& event) override;

    std::string_view getStateType() override { return TYPE; }
};
} // namespace Application
