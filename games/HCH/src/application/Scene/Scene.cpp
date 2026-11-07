#include "application/Scene/Scene.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/EntityFactory.hpp"
#include "core/ecs/Entity.hpp"
#include "application/Scene/ECS/Systems/Systems.hpp"

#include <memory>

const std::string Application::Scene::TYPE_NAME = "SCENE";

Application::Scene::Scene(sf::RenderWindow& window, const GameTuning& tuning)
    : mWindow(window), mEntityManager() {
    // Shared state read by the systems; tuning must exist before any entity is created
    mEntityManager.registry().ctx().emplace<GameTuning>(tuning);
    auto& stats = mEntityManager.registry().ctx().emplace<Component::GameStats>();
    stats.timeUntilNextWave = tuning.waves.firstDelay;

    initializeGameSystems();

    initializePlayerEntity();
}

void Application::Scene::initializeGameSystems() {
    // Order matters: input -> AI -> movement -> combat -> health -> spawning
    mEntityManager.registerSystem(std::make_unique<System::PlayerInputSystem>());
    mEntityManager.registerSystem(std::make_unique<System::EnemyAISystem>());
    mEntityManager.registerSystem(std::make_unique<System::MovementSystem>());
    mEntityManager.registerSystem(std::make_unique<System::CombatSystem>());
    mEntityManager.registerSystem(std::make_unique<System::HealthSystem>());
    mEntityManager.registerSystem(std::make_unique<System::SpawnSystem>());

    mEntityManager.registerRenderingSystem(std::make_unique<System::RenderSystem>(mWindow));
}

void Application::Scene::initializePlayerEntity() {
    EntityFactory::createPlayer(mEntityManager.registry(),
                                mEntityManager.registry().ctx().get<GameTuning>().worldSize() /
                                    2.f);
}

void Application::Scene::applyAction(Actions::Player action) {
    auto view = mEntityManager.registry().view<Component::PlayerInputComponent>();

    for (auto entity : view) {
        auto& input = view.get<Component::PlayerInputComponent>(entity);

        switch (action) {
        case Actions::Player::MOVE_UP:
            input.direction.y -= 1.f;
            break;
        case Actions::Player::MOVE_DOWN:
            input.direction.y += 1.f;
            break;
        case Actions::Player::MOVE_LEFT:
            input.direction.x -= 1.f;
            break;
        case Actions::Player::MOVE_RIGHT:
            input.direction.x += 1.f;
            break;
        case Actions::Player::ATTACK:
            input.attackRequested = true;
            break;
        case Actions::Player::NONE:
            break;
        }
    }
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
