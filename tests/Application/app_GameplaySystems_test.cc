#include <gtest/gtest.h>

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/EntityFactory.hpp"
#include "application/Scene/ECS/Systems/CombatSystem.hpp"
#include "application/Scene/ECS/Systems/EnemyAISystem.hpp"
#include "application/Scene/ECS/Systems/HealthSystem.hpp"
#include "application/Scene/ECS/Systems/MovementSystem.hpp"
#include "application/Scene/ECS/Systems/SpawnSystem.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

namespace {
const sf::Time STEP = sf::seconds(1.f / 120.f);

// Registry with the shared stats the systems expect
struct World {
    explicit World(const Application::GameTuning& tuning = Application::GameTuning()) {
        registry.ctx().emplace<Application::GameTuning>(tuning);
        registry.ctx().emplace<Component::GameStats>().timeUntilNextWave = tuning.waves.firstDelay;
    }

    template <typename T> T& system() {
        auto sys = std::make_unique<T>();
        sys->init(registry);
        T& ref = *sys;
        owned.push_back(std::move(sys));
        return ref;
    }

    Component::GameStats& stats() { return registry.ctx().get<Component::GameStats>(); }

    entt::registry registry;
    std::vector<std::unique_ptr<Core::BaseSystem>> owned;
};

int countEnemies(entt::registry& registry) {
    int count = 0;
    for (auto entity : registry.view<Component::Enemy>()) {
        (void)entity;
        ++count;
    }
    return count;
}
} // namespace

TEST(App_CombatSystem_Test, playerAttackDamagesAndKnocksBackEnemy) {
    World world;
    auto& combat = world.system<System::CombatSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {500.f, 500.f});
    // Player faces +x by default? Force it, then place an enemy in reach
    world.registry.get<Component::Transform>(player).facing = {1.f, 0.f};
    auto enemy = Application::EntityFactory::createEnemy(world.registry, {540.f, 500.f}, 1);
    const float startHealth = world.registry.get<Component::Health>(enemy).current;

    world.registry.get<Component::MeleeAttacker>(player).wantsAttack = true;
    combat.update(STEP);

    EXPECT_LT(world.registry.get<Component::Health>(enemy).current, startHealth);
    EXPECT_GT(world.registry.get<Component::Velocity>(enemy).knockback.x, 0.f);
}

TEST(App_CombatSystem_Test, attackOnCooldownDoesNothing) {
    World world;
    auto& combat = world.system<System::CombatSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {500.f, 500.f});
    world.registry.get<Component::Transform>(player).facing = {1.f, 0.f};
    auto enemy = Application::EntityFactory::createEnemy(world.registry, {540.f, 500.f}, 1);

    auto& attacker = world.registry.get<Component::MeleeAttacker>(player);
    attacker.cooldownTimer = 5.f;
    attacker.wantsAttack = true;
    combat.update(STEP);

    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(enemy).current,
                    world.registry.get<Component::Health>(enemy).max);
    EXPECT_FALSE(attacker.wantsAttack); // Request is consumed either way
}

TEST(App_CombatSystem_Test, invulnerabilityPreventsRepeatedHitsFromSameSwing) {
    World world;
    auto& combat = world.system<System::CombatSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {500.f, 500.f});
    world.registry.get<Component::Transform>(player).facing = {1.f, 0.f};
    auto enemy = Application::EntityFactory::createEnemy(world.registry, {540.f, 500.f}, 1);

    world.registry.get<Component::MeleeAttacker>(player).wantsAttack = true;
    combat.update(STEP);
    const float afterFirstHit = world.registry.get<Component::Health>(enemy).current;

    // The hitbox lives for several more steps but must not hit the same target again
    for (int i = 0; i < 5; ++i)
        combat.update(STEP);

    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(enemy).current, afterFirstHit);
}

TEST(App_CombatSystem_Test, hitboxExpiresAndIsDestroyed) {
    World world;
    auto& combat = world.system<System::CombatSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {500.f, 500.f});
    world.registry.get<Component::MeleeAttacker>(player).wantsAttack = true;
    combat.update(STEP);
    EXPECT_FALSE(world.registry.view<Component::Hitbox>().empty());

    for (int i = 0; i < 120; ++i) // one second, far longer than the hitbox lifetime
        combat.update(STEP);
    EXPECT_TRUE(world.registry.view<Component::Hitbox>().empty());
}

TEST(App_CombatSystem_Test, enemyAttackHurtsPlayerButNotOtherEnemies) {
    World world;
    auto& combat = world.system<System::CombatSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {500.f, 500.f});
    auto attackerEnemy = Application::EntityFactory::createEnemy(world.registry, {530.f, 500.f}, 1);
    auto bystander = Application::EntityFactory::createEnemy(world.registry, {545.f, 500.f}, 1);

    auto& attacker = world.registry.get<Component::MeleeAttacker>(attackerEnemy);
    attacker.cooldownTimer = 0.f;
    attacker.wantsAttack = true;
    world.registry.get<Component::Transform>(attackerEnemy).facing = {-1.f, 0.f};
    combat.update(STEP);

    EXPECT_LT(world.registry.get<Component::Health>(player).current,
              world.registry.get<Component::Health>(player).max);
    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(bystander).current,
                    world.registry.get<Component::Health>(bystander).max);
}

TEST(App_EnemyAISystem_Test, enemyMovesTowardPlayerAndAttacksInRange) {
    World world;
    auto& ai = world.system<System::EnemyAISystem>();

    Application::EntityFactory::createPlayer(world.registry, {1000.f, 700.f});
    auto far = Application::EntityFactory::createEnemy(world.registry, {600.f, 700.f}, 1);
    ai.update(STEP);

    EXPECT_GT(world.registry.get<Component::Velocity>(far).move.x, 0.f);
    EXPECT_FALSE(world.registry.get<Component::MeleeAttacker>(far).wantsAttack);

    world.registry.get<Component::Transform>(far).position = {1020.f, 700.f};
    ai.update(STEP);
    EXPECT_TRUE(world.registry.get<Component::MeleeAttacker>(far).wantsAttack);
}

TEST(App_EnemyAISystem_Test, noPlayerMeansEnemiesStandStill) {
    World world;
    auto& ai = world.system<System::EnemyAISystem>();

    auto enemy = Application::EntityFactory::createEnemy(world.registry, {600.f, 700.f}, 1);
    world.registry.get<Component::Velocity>(enemy).move = {50.f, 50.f};
    ai.update(STEP);

    EXPECT_FLOAT_EQ(world.registry.get<Component::Velocity>(enemy).move.x, 0.f);
    EXPECT_FLOAT_EQ(world.registry.get<Component::Velocity>(enemy).move.y, 0.f);
}

TEST(App_HealthSystem_Test, deadEnemyIsRemovedAndScored) {
    World world;
    auto& health = world.system<System::HealthSystem>();

    auto enemy = Application::EntityFactory::createEnemy(world.registry, {600.f, 700.f}, 1);
    world.registry.get<Component::Health>(enemy).current = 0.f;
    health.update(STEP);

    EXPECT_EQ(countEnemies(world.registry), 0);
    EXPECT_EQ(world.stats().score, 10);
    EXPECT_FALSE(world.stats().playerDead);
}

TEST(App_HealthSystem_Test, deadPlayerIsFlaggedNotDestroyed) {
    World world;
    auto& health = world.system<System::HealthSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {600.f, 700.f});
    world.registry.get<Component::Health>(player).current = -5.f;
    health.update(STEP);

    EXPECT_TRUE(world.stats().playerDead);
    EXPECT_TRUE(world.registry.valid(player));
}

TEST(App_HealthSystem_Test, invulnerabilityTimerCountsDown) {
    World world;
    auto& health = world.system<System::HealthSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {600.f, 700.f});
    world.registry.get<Component::Health>(player).invulnerableTimer = 0.1f;
    for (int i = 0; i < 60; ++i)
        health.update(STEP);

    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(player).invulnerableTimer, 0.f);
}

TEST(App_MovementSystem_Test, movesAndClampsToWorld) {
    World world;
    auto& movement = world.system<System::MovementSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {100.f, 100.f});
    world.registry.get<Component::Velocity>(player).move = {120.f, 0.f};
    movement.update(sf::seconds(0.5f));
    EXPECT_NEAR(world.registry.get<Component::Transform>(player).position.x, 160.f, 0.01f);

    world.registry.get<Component::Velocity>(player).move = {-100000.f, -100000.f};
    movement.update(sf::seconds(1.f));
    EXPECT_GE(world.registry.get<Component::Transform>(player).position.x, 0.f);
    EXPECT_GE(world.registry.get<Component::Transform>(player).position.y, 0.f);
}

TEST(App_MovementSystem_Test, knockbackDecays) {
    World world;
    auto& movement = world.system<System::MovementSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {1000.f, 700.f});
    world.registry.get<Component::Velocity>(player).knockback = {500.f, 0.f};
    for (int i = 0; i < 240; ++i)
        movement.update(STEP);

    EXPECT_LT(world.registry.get<Component::Velocity>(player).knockback.x, 1.f);
}

TEST(App_SpawnSystem_Test, spawnsGrowingWavesOnlyWhenCleared) {
    const Application::GameTuning tuning;
    World world(tuning);
    auto& spawn = world.system<System::SpawnSystem>();

    Application::EntityFactory::createPlayer(world.registry, {1000.f, 700.f});

    // Run long enough to pass the initial delay
    for (int i = 0; i < 400; ++i)
        spawn.update(STEP);
    EXPECT_EQ(world.stats().wave, 1);
    EXPECT_EQ(countEnemies(world.registry), tuning.waves.baseEnemies);

    // Enemies alive: no new wave
    for (int i = 0; i < 1000; ++i)
        spawn.update(STEP);
    EXPECT_EQ(world.stats().wave, 1);

    // Clear them: the next wave is bigger
    std::vector<entt::entity> enemies;
    for (auto entity : world.registry.view<Component::Enemy>())
        enemies.push_back(entity);
    for (auto entity : enemies)
        world.registry.destroy(entity);

    for (int i = 0; i < 400; ++i)
        spawn.update(STEP);
    EXPECT_EQ(world.stats().wave, 2);
    EXPECT_EQ(countEnemies(world.registry), tuning.waves.baseEnemies + tuning.waves.enemiesPerWave);
}

TEST(App_SpawnSystem_Test, spawnedEnemiesKeepDistanceFromPlayer) {
    const Application::GameTuning tuning;
    World world(tuning);
    auto& spawn = world.system<System::SpawnSystem>();

    const sf::Vector2f playerPos{1000.f, 700.f};
    Application::EntityFactory::createPlayer(world.registry, playerPos);
    for (int i = 0; i < 400; ++i)
        spawn.update(STEP);

    for (auto entity : world.registry.view<Component::Enemy, Component::Transform>()) {
        const sf::Vector2f offset =
            world.registry.get<Component::Transform>(entity).position - playerPos;
        EXPECT_GE(std::sqrt(offset.x * offset.x + offset.y * offset.y),
                  tuning.waves.minSpawnDistance - 0.01f);
    }
}

TEST(App_SpawnSystem_Test, noSpawningAfterPlayerDeath) {
    World world;
    auto& spawn = world.system<System::SpawnSystem>();

    Application::EntityFactory::createPlayer(world.registry, {1000.f, 700.f});
    world.stats().playerDead = true;
    for (int i = 0; i < 600; ++i)
        spawn.update(STEP);

    EXPECT_EQ(countEnemies(world.registry), 0);
}

// ---- Tuning flows through to the systems -------------------------------------------------------

TEST(App_SpawnSystem_Test, waveSizeComesFromTuning) {
    Application::GameTuning tuning;
    tuning.waves.baseEnemies = 7;
    tuning.waves.firstDelay = 0.f;
    World world(tuning);
    auto& spawn = world.system<System::SpawnSystem>();

    Application::EntityFactory::createPlayer(world.registry, {1000.f, 700.f});
    spawn.update(STEP);

    EXPECT_EQ(countEnemies(world.registry), 7);
}

TEST(App_EntityFactory_Test, playerAndEnemyStatsComeFromTuning) {
    Application::GameTuning tuning;
    tuning.player.health = 250.f;
    tuning.player.attackReach = 77.f;
    tuning.enemy.baseHealth = 50.f;
    tuning.enemy.healthPerWave = 10.f;
    tuning.enemy.baseSpeed = 100.f;
    tuning.enemy.speedPerWave = 20.f;
    tuning.enemy.maxSpeed = 120.f;
    World world(tuning);

    auto player = Application::EntityFactory::createPlayer(world.registry, {0.f, 0.f});
    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(player).max, 250.f);
    EXPECT_FLOAT_EQ(world.registry.get<Component::MeleeAttacker>(player).reach, 77.f);

    auto enemy = Application::EntityFactory::createEnemy(world.registry, {0.f, 0.f}, 3);
    EXPECT_FLOAT_EQ(world.registry.get<Component::Health>(enemy).max, 80.f);     // 50 + 3 * 10
    EXPECT_FLOAT_EQ(world.registry.get<Component::EnemyAI>(enemy).speed, 120.f); // capped
}

TEST(App_MovementSystem_Test, worldSizeAndKnockbackDecayComeFromTuning) {
    Application::GameTuning tuning;
    tuning.world.width = 500.f;
    tuning.knockbackDecay = 0.f; // Knockback never fades
    World world(tuning);
    auto& movement = world.system<System::MovementSystem>();

    auto player = Application::EntityFactory::createPlayer(world.registry, {100.f, 100.f});
    world.registry.get<Component::Velocity>(player).move = {100000.f, 0.f};
    movement.update(sf::seconds(1.f));
    EXPECT_LE(world.registry.get<Component::Transform>(player).position.x, 500.f);

    world.registry.get<Component::Velocity>(player).move = {0.f, 0.f};
    world.registry.get<Component::Velocity>(player).knockback = {10.f, 0.f};
    for (int i = 0; i < 240; ++i)
        movement.update(STEP);
    EXPECT_FLOAT_EQ(world.registry.get<Component::Velocity>(player).knockback.x, 10.f);
}
