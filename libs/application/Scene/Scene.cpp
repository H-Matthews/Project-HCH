#include "application/Scene/Scene.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/Entity.hpp"
#include "application/Scene/ECS/Systems/Systems.hpp"

#include <memory>

const std::string Application::Scene::TYPE_NAME = "SCENE";

Application::Scene::Scene(sf::RenderWindow& window, Core::MessageNetwork& gameNetwork,
                          const GameTuning& tuning)
    : mWindow(window), mEntityManager() {
    // Shared state read by the systems; tuning must exist before any entity is created
    mEntityManager.registry().ctx().emplace<GameTuning>(tuning);
    auto& stats = mEntityManager.registry().ctx().emplace<Component::GameStats>();
    stats.timeUntilNextWave = tuning.waves.firstDelay;

    initializeGameSystems(gameNetwork);

    initializePlayerEntity();
}

void Application::Scene::initializeGameSystems(Core::MessageNetwork& gameNetwork) {
    // Order matters: input -> AI -> movement -> combat -> health -> spawning
    mEntityManager.registerSystem(std::make_unique<System::PlayerInputSystem>(gameNetwork));
    mEntityManager.registerSystem(std::make_unique<System::EnemyAISystem>());
    mEntityManager.registerSystem(std::make_unique<System::MovementSystem>());
    mEntityManager.registerSystem(std::make_unique<System::CombatSystem>());
    mEntityManager.registerSystem(std::make_unique<System::HealthSystem>());
    mEntityManager.registerSystem(std::make_unique<System::SpawnSystem>());

    mEntityManager.registerRenderingSystem(std::make_unique<System::RenderSystem>(mWindow));
}

void Application::Scene::initializePlayerEntity() {
    mEntityManager.createPlayerEntity(
        mEntityManager.registry().ctx().get<GameTuning>().worldSize() / 2.f);
}

void Application::Scene::update(sf::Time fixedTimeStep) {
    mEntityManager.update(fixedTimeStep);
}

void Application::Scene::draw() {
    mEntityManager.render();
}

const Component::GameStats& Application::Scene::stats() const {
    return mEntityManager.registry().ctx().get<Component::GameStats>();
}

float Application::Scene::playerHealth() const {
    for (auto entity : mEntityManager.registry().view<Component::Player, Component::Health>())
        return mEntityManager.registry().get<Component::Health>(entity).current;
    return 0.f;
}

float Application::Scene::playerMaxHealth() const {
    for (auto entity : mEntityManager.registry().view<Component::Player, Component::Health>())
        return mEntityManager.registry().get<Component::Health>(entity).max;
    return 0.f;
}
