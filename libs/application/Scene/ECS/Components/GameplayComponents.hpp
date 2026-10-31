#pragma once

#include "vendor/entt/include/entt.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

/**
 * Plain-data components used by the gameplay systems. Kept header-only; they carry no behaviour.
 */
namespace Component {

// Tags
struct Player {};
struct Enemy {};

struct Transform {
    sf::Vector2f position{0.f, 0.f};
    sf::Vector2f facing{1.f, 0.f}; // Always normalized
};

struct Velocity {
    sf::Vector2f move{0.f, 0.f};      // Set each step by input / AI (units per second)
    sf::Vector2f knockback{0.f, 0.f}; // Impulse that decays over time
};

struct Collider {
    float radius = 10.f;
};

struct Health {
    float current = 100.f;
    float max = 100.f;
    float invulnerableTimer = 0.f;     // Seconds of damage immunity remaining
    float invulnerableDuration = 0.3f; // Applied whenever damage is taken
};

enum class TeamID { PLAYER, ENEMY };
struct Team {
    TeamID id = TeamID::PLAYER;
};

// Drawn as a filled circle. Higher layers are drawn on top.
struct Shape {
    float radius = 10.f;
    sf::Color color = sf::Color::White;
    int layer = 1;
};

// Anything that can swing a melee attack (player and enemies)
struct MeleeAttacker {
    float damage = 10.f;
    float cooldown = 0.5f;
    float cooldownTimer = 0.f;
    float reach = 30.f;       // Distance from the attacker to the centre of the hitbox
    float hitRadius = 24.f;   // Radius of the hitbox
    float knockback = 400.f;  // Impulse applied to what it hits
    bool wantsAttack = false; // Set by input / AI, consumed by CombatSystem
};

// Short-lived damage area spawned by an attack
struct Hitbox {
    TeamID team = TeamID::PLAYER;
    float damage = 10.f;
    float knockback = 400.f;
    float radius = 24.f;
    float lifetime = 0.12f;
    std::vector<entt::entity> alreadyHit;
};

struct EnemyAI {
    float speed = 110.f;
    float attackDistance = 46.f; // Centre-to-centre distance at which it starts attacking
};

// Shared game state stored in the registry context (registry.ctx())
struct GameStats {
    int wave = 0;
    int score = 0;
    bool playerDead = false;
    float timeUntilNextWave = 1.5f;
};

} // namespace Component
