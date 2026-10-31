#pragma once

#include <SFML/Graphics.hpp>

#include "core/Messaging/MessageNetwork.hpp"

#include "application/Scene/ECS/Components/GameplayComponents.hpp"
#include "application/Scene/ECS/EntityManager.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

namespace Application {

/**
 * High Level class that represents a "Scene" during gameplay. It is used by the Game State to drive
 * updates to our Entities
 */
class Scene {
  public:
    static const std::string TYPE_NAME;

  public:
    explicit Scene(sf::RenderWindow& window, Core::MessageNetwork& gameNetwork,
                   const GameTuning& tuning = GameTuning());

    void update(sf::Time fixedTimeStep);
    void draw();

    // Game progress (wave, score, death) shared by the systems through the registry context
    const Component::GameStats& stats() const;

    // Player health for the HUD; both 0 if there is no player
    float playerHealth() const;
    float playerMaxHealth() const;

  private:
    void initializeGameSystems(Core::MessageNetwork& gameNetwork);
    void initializePlayerEntity();

  private:
    sf::RenderWindow& mWindow;

    EntityManager mEntityManager;
};
} // namespace Application
