#include "application/Scene/ECS/EntityFactory.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

#include <algorithm>

entt::entity Application::EntityFactory::createPlayer(entt::registry& registry,
                                                      sf::Vector2f position) {
    const auto& tuning = registry.ctx().get<GameTuning>();
    const entt::entity entity = registry.create();

    registry.emplace<Component::Player>(entity);
    registry.emplace<Component::PlayerInputComponent>(entity);
    registry.emplace<Component::Transform>(entity, position, sf::Vector2f{0.f, 1.f});
    registry.emplace<Component::Velocity>(entity);
    registry.emplace<Component::Collider>(entity, tuning.player.radius);
    registry.emplace<Component::Team>(entity, Component::TeamID::PLAYER);
    registry.emplace<Component::Shape>(entity, tuning.player.radius, sf::Color(70, 140, 255), 2);

    Component::Health health;
    health.current = health.max = tuning.player.health;
    health.invulnerableDuration = tuning.player.invulnerable;
    registry.emplace<Component::Health>(entity, health);

    Component::MeleeAttacker attacker;
    attacker.damage = tuning.player.damage;
    attacker.cooldown = tuning.player.attackCooldown;
    attacker.reach = tuning.player.attackReach;
    attacker.hitRadius = tuning.player.attackRadius;
    attacker.knockback = tuning.player.knockback;
    registry.emplace<Component::MeleeAttacker>(entity, attacker);

    return entity;
}

entt::entity Application::EntityFactory::createEnemy(entt::registry& registry,
                                                     sf::Vector2f position, int wave) {
    const auto& tuning = registry.ctx().get<GameTuning>();
    const entt::entity entity = registry.create();

    registry.emplace<Component::Enemy>(entity);
    registry.emplace<Component::Transform>(entity, position, sf::Vector2f{0.f, 1.f});
    registry.emplace<Component::Velocity>(entity);
    registry.emplace<Component::Collider>(entity, tuning.enemy.radius);
    registry.emplace<Component::Team>(entity, Component::TeamID::ENEMY);
    registry.emplace<Component::Shape>(entity, tuning.enemy.radius, sf::Color(220, 70, 70), 1);

    Component::Health health;
    health.current = health.max =
        tuning.enemy.baseHealth + tuning.enemy.healthPerWave * static_cast<float>(wave);
    health.invulnerableDuration = tuning.enemy.invulnerable;
    registry.emplace<Component::Health>(entity, health);

    Component::EnemyAI ai;
    ai.attackDistance = tuning.enemy.attackDistance;
    ai.speed =
        std::min(tuning.enemy.baseSpeed + tuning.enemy.speedPerWave * static_cast<float>(wave),
                 tuning.enemy.maxSpeed);
    registry.emplace<Component::EnemyAI>(entity, ai);

    Component::MeleeAttacker attacker;
    attacker.damage = tuning.enemy.damage;
    attacker.cooldown = tuning.enemy.attackCooldown;
    attacker.cooldownTimer = tuning.enemy.firstAttackDelay;
    attacker.reach = tuning.enemy.attackReach;
    attacker.hitRadius = tuning.enemy.attackRadius;
    attacker.knockback = tuning.enemy.knockback;
    registry.emplace<Component::MeleeAttacker>(entity, attacker);

    return entity;
}

entt::entity Application::EntityFactory::createHitbox(entt::registry& registry,
                                                      sf::Vector2f position, Component::TeamID team,
                                                      float damage, float knockback, float radius) {
    const entt::entity entity = registry.create();

    Component::Hitbox hitbox;
    hitbox.team = team;
    hitbox.damage = damage;
    hitbox.knockback = knockback;
    hitbox.radius = radius;

    registry.emplace<Component::Transform>(entity, position);
    registry.emplace<Component::Hitbox>(entity, std::move(hitbox));
    registry.emplace<Component::Shape>(entity, radius, sf::Color(255, 220, 90, 110), 3);

    return entity;
}
