#include "application/Scene/ECS/Systems/EnemyAISystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/Systems/VectorMath.hpp"

#include <vector>

void System::EnemyAISystem::update(const sf::Time) {
    // Find the player (there is at most one)
    bool havePlayer = false;
    sf::Vector2f playerPosition;
    for (auto entity : mRegistry->view<Component::Player, Component::Transform>()) {
        playerPosition = mRegistry->get<Component::Transform>(entity).position;
        havePlayer = true;
        break;
    }

    auto enemies =
        mRegistry->view<Component::Enemy, Component::EnemyAI, Component::Transform,
                        Component::Velocity, Component::MeleeAttacker, Component::Collider>();

    if (!havePlayer) {
        for (auto entity : enemies)
            enemies.get<Component::Velocity>(entity).move = {0.f, 0.f};
        return;
    }

    // Snapshot enemy positions for the (small N) separation pass
    std::vector<std::pair<entt::entity, sf::Vector2f>> others;
    for (auto entity : enemies)
        others.emplace_back(entity, enemies.get<Component::Transform>(entity).position);

    for (auto entity : enemies) {
        auto& ai = enemies.get<Component::EnemyAI>(entity);
        auto& transform = enemies.get<Component::Transform>(entity);
        auto& velocity = enemies.get<Component::Velocity>(entity);
        auto& attacker = enemies.get<Component::MeleeAttacker>(entity);
        const float radius = enemies.get<Component::Collider>(entity).radius;

        const sf::Vector2f toPlayer = playerPosition - transform.position;
        const float distance = Math::length(toPlayer);
        const sf::Vector2f direction = Math::normalized(toPlayer);

        if (distance > 0.001f)
            transform.facing = direction;

        // Stop moving once in striking range so enemies don't stack on the player
        sf::Vector2f desired =
            distance > ai.attackDistance * 0.8f ? direction : sf::Vector2f{0.f, 0.f};

        // Push away from overlapping neighbours
        sf::Vector2f separation{0.f, 0.f};
        for (const auto& [otherEntity, otherPosition] : others) {
            if (otherEntity == entity)
                continue;
            const sf::Vector2f away = transform.position - otherPosition;
            const float d = Math::length(away);
            if (d > 0.001f && d < radius * 2.5f)
                separation += Math::normalized(away) * ((radius * 2.5f - d) / (radius * 2.5f));
        }

        velocity.move = Math::normalized(desired + separation * 1.5f) * ai.speed;

        if (distance <= ai.attackDistance)
            attacker.wantsAttack = true;
    }
}
