#pragma once

#include "application/Scene/Scene.hpp"
#include "application/Messaging/Publishers/PlayerInputPublisher.hpp"
#include "application/Settings/KeyBindings.hpp"

#include "core/StateStack/State.hpp"

#include <memory>
#include <string>

namespace Application {

/**
 * High Level class that contains everything the game would need to run when playing the game
 */
class GameState : public Core::State {
  public:
    static std::string IDENTIFIER;

    GameState();

    bool initializeState() override;

    virtual void draw() override;
    virtual bool update(sf::Time fixedTimeStep) override;
    bool handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) override;
    bool handleRealTimeInput() override;

    ~GameState();

  private:
    void drawHud();

  private:
    Application::KeyBindings mKeyBindings;

    // Destruction order matters: the publisher unregisters from the network, and the scene owns
    // the subscriber system, so both are released before the state goes away
    std::unique_ptr<Scene> mScene;
    std::unique_ptr<PlayerInputPublisher> mPlayerInputPublisher;

    bool mGameOverRequested = false;
};
} // namespace Application
