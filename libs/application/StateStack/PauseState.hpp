#pragma once

#include "core/StateStack/State.hpp"

#include <string>

namespace Application {
class PauseState : public Core::State {
  public:
    static std::string IDENTIFIER;

    PauseState();

    virtual void draw() override;

    virtual bool update(sf::Time fixedTimeStep) override;
    bool handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) override;
};
} // namespace Application
