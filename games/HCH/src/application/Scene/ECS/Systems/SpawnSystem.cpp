#include "application/Scene/ECS/Systems/SpawnSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/EntityFactory.hpp"
#include "utility/VectorMath.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

System::SpawnSystem::SpawnSystem() : mRng(std::random_device{}()) {}

sf::Vector2f System::SpawnSystem::pickSpawnPosition(sf::Vector2f playerPosition) {
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    constexpr float EDGE_MARGIN = 40.f;
    std::uniform_real_distribution<float> xDist(EDGE_MARGIN, tuning.world.width - EDGE_MARGIN);
    std::uniform_real_distribution<float> yDist(EDGE_MARGIN, tuning.world.height - EDGE_MARGIN);

    sf::Vector2f candidate;
    for (int attempt = 0; attempt < 30; ++attempt) {
        candidate = {xDist(mRng), yDist(mRng)};
        if (Utility::Math::length(candidate - playerPosition) >= tuning.waves.minSpawnDistance) {
            return candidate;
        }
    }

    // Fall back to the corner furthest from the player
    return {playerPosition.x < tuning.world.width / 2.f ? tuning.world.width - EDGE_MARGIN
                                                        : EDGE_MARGIN,
            playerPosition.y < tuning.world.height / 2.f ? tuning.world.height - EDGE_MARGIN
                                                         : EDGE_MARGIN};
}

void System::SpawnSystem::update(const sf::Time deltaTime) {
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    auto& stats = mRegistry->ctx().get<Component::GameStats>();
    if (stats.playerDead) {
        return;
    }

    if (!mRegistry->view<Component::Enemy>().empty()) {
        return;
    }

    stats.timeUntilNextWave -= deltaTime.asSeconds();
    if (stats.timeUntilNextWave > 0.f) {
        return;
    }

    sf::Vector2f playerPosition = tuning.worldSize() / 2.f;
    for (auto entity : mRegistry->view<Component::Player, Component::Transform>()) {
        playerPosition = mRegistry->get<Component::Transform>(entity).position;
        break;
    }

    ++stats.wave;
    const int count = tuning.waves.baseEnemies + tuning.waves.enemiesPerWave * (stats.wave - 1);
    for (int i = 0; i < count; ++i) {
        Application::EntityFactory::createEnemy(*mRegistry, pickSpawnPosition(playerPosition),
                                                stats.wave);
    }

    stats.timeUntilNextWave = tuning.waves.delay;
}
