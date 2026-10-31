#include "application/Scene/ECS/Systems/HealthSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"

#include <algorithm>
#include <vector>

void System::HealthSystem::update(const sf::Time deltaTime) {
    const float dt = deltaTime.asSeconds();

    auto& stats = mRegistry->ctx().get<Component::GameStats>();
    std::vector<entt::entity> dead;

    auto view = mRegistry->view<Component::Health>();
    for (auto entity : view) {
        auto& health = view.get<Component::Health>(entity);
        health.invulnerableTimer = std::max(0.f, health.invulnerableTimer - dt);

        if (health.current > 0.f)
            continue;

        health.current = 0.f;

        if (mRegistry->all_of<Component::Player>(entity))
            stats.playerDead = true;
        else
            dead.push_back(entity);
    }

    for (auto entity : dead) {
        if (mRegistry->all_of<Component::Enemy>(entity))
            stats.score += 10;
        mRegistry->destroy(entity);
    }
}
