#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <SFML/System/Vector2.hpp>

namespace Application {

/**
 * All gameplay balance values. Member initializers are the built-in defaults; fromConfig() lets
 * the [Gameplay] table in configs/gameplay.toml override any of them. A missing section, missing
 * key or wrongly typed value silently keeps the default, and out-of-range values are clamped.
 *
 * Stored in the registry context, so systems read it with `registry.ctx().get<GameTuning>()`.
 */
struct GameTuning {
    static constexpr std::string_view SECTION_NAME = "Gameplay";

    struct World {
        float width = 2000.f;
        float height = 1400.f;
    } world;

    struct Player {
        float speed = 260.f;
        float radius = 16.f;
        float health = 100.f;
        float damage = 12.f;
        float attackCooldown = 0.35f;
        float attackReach = 34.f;
        float attackRadius = 30.f;
        float knockback = 520.f;
        float invulnerable = 0.6f; // Seconds of damage immunity after being hit
    } player;

    struct Enemy {
        float radius = 14.f;
        float baseHealth = 24.f;
        float healthPerWave = 4.f;
        float baseSpeed = 95.f;
        float speedPerWave = 6.f;
        float maxSpeed = 190.f;
        float damage = 8.f;
        float attackCooldown = 1.2f;
        float firstAttackDelay = 0.6f; // Grace period so a fresh spawn doesn't hit instantly
        float attackReach = 26.f;
        float attackRadius = 22.f;
        float attackDistance = 46.f; // Distance to the player at which it starts attacking
        float knockback = 300.f;
        float invulnerable = 0.15f;
    } enemy;

    struct Waves {
        int baseEnemies = 3;
        int enemiesPerWave = 2;
        float delay = 2.0f;             // Seconds between clearing a wave and the next one
        float firstDelay = 1.5f;        // Seconds before the very first wave
        float minSpawnDistance = 450.f; // Enemies never spawn closer than this to the player
    } waves;

    float knockbackDecay = 9.f; // Higher = knockback fades faster

    sf::Vector2f worldSize() const { return {world.width, world.height}; }

    /// `section` may be null (everything stays default).
    static GameTuning fromConfig(const Core::ConfigSection* section);
};

} // namespace Application
