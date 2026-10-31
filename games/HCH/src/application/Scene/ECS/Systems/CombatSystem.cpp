#include "application/Scene/ECS/Systems/CombatSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/EntityFactory.hpp"
#include "utility/VectorMath.hpp"

#include <algorithm>
#include <vector>

void System::CombatSystem::update(const sf::Time deltaTime) {
    const float dt = deltaTime.asSeconds();

    spawnHitboxes(dt);
    resolveHitboxes(dt);
}

void System::CombatSystem::spawnHitboxes(float dt) {
    struct Spawn {
        sf::Vector2f position;
        Component::TeamID team;
        const Component::MeleeAttacker attacker;
    };
    std::vector<Spawn> spawns;

    auto view = mRegistry->view<Component::MeleeAttacker, Component::Transform, Component::Team>();

    for (auto entity : view) {
        auto& attacker = view.get<Component::MeleeAttacker>(entity);
        const auto& transform = view.get<Component::Transform>(entity);

        attacker.cooldownTimer = std::max(0.f, attacker.cooldownTimer - dt);

        if (attacker.wantsAttack && attacker.cooldownTimer <= 0.f) {
            attacker.cooldownTimer = attacker.cooldown;
            spawns.push_back({transform.position + transform.facing * attacker.reach,
                              view.get<Component::Team>(entity).id, attacker});
        }

        attacker.wantsAttack = false;
    }

    // Created after iterating so we never modify the registry mid-iteration
    for (const auto& spawn : spawns) {
        Application::EntityFactory::createHitbox(*mRegistry, spawn.position, spawn.team,
                                                 spawn.attacker.damage, spawn.attacker.knockback,
                                                 spawn.attacker.hitRadius);
    }
}

void System::CombatSystem::resolveHitboxes(float dt) {
    std::vector<entt::entity> expired;

    auto hitboxes = mRegistry->view<Component::Hitbox, Component::Transform>();
    auto targets =
        mRegistry
            ->view<Component::Health, Component::Team, Component::Transform, Component::Collider>();

    for (auto hitboxEntity : hitboxes) {
        auto& hitbox = hitboxes.get<Component::Hitbox>(hitboxEntity);
        const sf::Vector2f origin = hitboxes.get<Component::Transform>(hitboxEntity).position;

        for (auto target : targets) {
            if (targets.get<Component::Team>(target).id == hitbox.team) {
                continue;
            }

            if (std::find(hitbox.alreadyHit.begin(), hitbox.alreadyHit.end(), target) !=
                hitbox.alreadyHit.end()) {
                continue;
            }

            auto& health = targets.get<Component::Health>(target);
            if (health.invulnerableTimer > 0.f || health.current <= 0.f) {
                continue;
            }

            const sf::Vector2f offset = targets.get<Component::Transform>(target).position - origin;
            const float reach = hitbox.radius + targets.get<Component::Collider>(target).radius;
            if (Utility::Math::length(offset) > reach) {
                continue;
            }

            hitbox.alreadyHit.push_back(target);
            health.current -= hitbox.damage;
            health.invulnerableTimer = health.invulnerableDuration;

            if (auto* velocity = mRegistry->try_get<Component::Velocity>(target)) {
                sf::Vector2f push = Utility::Math::normalized(offset);
                if (push.x == 0.f && push.y == 0.f) {
                    push = {1.f, 0.f};
                }
                velocity->knockback = push * hitbox.knockback;
            }
        }

        hitbox.lifetime -= dt;
        if (hitbox.lifetime <= 0.f) {
            expired.push_back(hitboxEntity);
        }
    }

    for (auto entity : expired)
        mRegistry->destroy(entity);
}
