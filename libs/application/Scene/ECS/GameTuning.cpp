#include "application/Scene/ECS/GameTuning.hpp"

#include <algorithm>
#include <cmath>
#include <memory>

namespace {

// Reads `key` as a number into `target`, clamped to [lo, hi]. Anything that isn't a finite number
// (missing key, wrong type, NaN/inf) leaves `target` at its default.
void readFloat(const Core::ConfigSection* section, std::string_view key, float& target, float lo,
               float hi) {
    if (!section)
        return;

    const auto value = section->getDouble(key);
    if (!value || !std::isfinite(*value))
        return;

    target =
        static_cast<float>(std::clamp(*value, static_cast<double>(lo), static_cast<double>(hi)));
}

void readInt(const Core::ConfigSection* section, std::string_view key, int& target, int lo,
             int hi) {
    if (!section)
        return;

    if (const auto value = section->getInt(key))
        target = std::clamp(*value, lo, hi);
}

// getSection() returns an owning pointer; keep it alive for the duration of the read
struct Sub {
    Sub(const Core::ConfigSection* parent, std::string_view name)
        : owned(parent ? parent->getSection(name) : nullptr) {}
    const Core::ConfigSection* get() const { return owned.get(); }

    std::unique_ptr<Core::ConfigSection> owned;
};

} // namespace

Application::GameTuning Application::GameTuning::fromConfig(const Core::ConfigSection* section) {
    GameTuning t; // Defaults

    if (!section)
        return t;

    constexpr float BIG = 1.0e6f;

    readFloat(section, "knockback_decay", t.knockbackDecay, 0.f, 100.f);

    {
        Sub sub(section, "World");
        readFloat(sub.get(), "width", t.world.width, 400.f, 20000.f);
        readFloat(sub.get(), "height", t.world.height, 400.f, 20000.f);
    }
    {
        Sub sub(section, "Player");
        auto* s = sub.get();
        readFloat(s, "speed", t.player.speed, 0.f, BIG);
        readFloat(s, "radius", t.player.radius, 1.f, 500.f);
        readFloat(s, "health", t.player.health, 1.f, BIG);
        readFloat(s, "damage", t.player.damage, 0.f, BIG);
        readFloat(s, "attack_cooldown", t.player.attackCooldown, 0.f, 60.f);
        readFloat(s, "attack_reach", t.player.attackReach, 0.f, 2000.f);
        readFloat(s, "attack_radius", t.player.attackRadius, 1.f, 2000.f);
        readFloat(s, "knockback", t.player.knockback, 0.f, BIG);
        readFloat(s, "invulnerable", t.player.invulnerable, 0.f, 60.f);
    }
    {
        Sub sub(section, "Enemy");
        auto* s = sub.get();
        readFloat(s, "radius", t.enemy.radius, 1.f, 500.f);
        readFloat(s, "base_health", t.enemy.baseHealth, 1.f, BIG);
        readFloat(s, "health_per_wave", t.enemy.healthPerWave, 0.f, BIG);
        readFloat(s, "base_speed", t.enemy.baseSpeed, 0.f, BIG);
        readFloat(s, "speed_per_wave", t.enemy.speedPerWave, 0.f, BIG);
        readFloat(s, "max_speed", t.enemy.maxSpeed, 0.f, BIG);
        readFloat(s, "damage", t.enemy.damage, 0.f, BIG);
        readFloat(s, "attack_cooldown", t.enemy.attackCooldown, 0.f, 60.f);
        readFloat(s, "first_attack_delay", t.enemy.firstAttackDelay, 0.f, 60.f);
        readFloat(s, "attack_reach", t.enemy.attackReach, 0.f, 2000.f);
        readFloat(s, "attack_radius", t.enemy.attackRadius, 1.f, 2000.f);
        readFloat(s, "attack_distance", t.enemy.attackDistance, 0.f, 2000.f);
        readFloat(s, "knockback", t.enemy.knockback, 0.f, BIG);
        readFloat(s, "invulnerable", t.enemy.invulnerable, 0.f, 60.f);
    }
    {
        Sub sub(section, "Waves");
        auto* s = sub.get();
        readInt(s, "base_enemies", t.waves.baseEnemies, 1, 500);
        readInt(s, "enemies_per_wave", t.waves.enemiesPerWave, 0, 500);
        readFloat(s, "delay", t.waves.delay, 0.f, 600.f);
        readFloat(s, "first_delay", t.waves.firstDelay, 0.f, 600.f);
        readFloat(s, "min_spawn_distance", t.waves.minSpawnDistance, 0.f, 20000.f);
    }

    // Keep derived values consistent so a partial edit can't produce an impossible setup
    t.enemy.maxSpeed = std::max(t.enemy.maxSpeed, t.enemy.baseSpeed);

    return t;
}
