#include <gtest/gtest.h>

#include "application/Scene/ECS/GameTuning.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"
#include "core/Configuration/ConfigSection/TOMLConfigSection.hpp"

#include "vendor/toml/include/toml.hpp"

namespace {
Application::GameTuning load(std::string_view tomlContent) {
    Core::TOMLConfigSection root(toml::parse(tomlContent));
    auto gameplay = root.getSection(Application::GameTuning::SECTION_NAME);
    return Application::GameTuning::fromConfig(gameplay.get());
}
} // namespace

TEST(App_GameTuning_Test, nullSectionGivesDefaults) {
    const Application::GameTuning defaults;
    const auto loaded = Application::GameTuning::fromConfig(nullptr);

    EXPECT_FLOAT_EQ(loaded.player.speed, defaults.player.speed);
    EXPECT_EQ(loaded.waves.baseEnemies, defaults.waves.baseEnemies);
}

TEST(App_GameTuning_Test, emptyGameplayTableGivesDefaults) {
    const Application::GameTuning defaults;
    const auto loaded = load("[Gameplay]\n");

    EXPECT_FLOAT_EQ(loaded.enemy.baseSpeed, defaults.enemy.baseSpeed);
    EXPECT_FLOAT_EQ(loaded.world.width, defaults.world.width);
}

TEST(App_GameTuning_Test, overridesAreApplied) {
    const auto loaded = load(R"(
        [Gameplay]
        knockback_decay = 4.5
        [Gameplay.Player]
        speed = 300.5
        health = 150
        [Gameplay.Enemy]
        damage = 20.0
        [Gameplay.Waves]
        base_enemies = 9
        delay = 0.5
        [Gameplay.World]
        width = 3000.0
    )");

    EXPECT_FLOAT_EQ(loaded.knockbackDecay, 4.5f);
    EXPECT_FLOAT_EQ(loaded.player.speed, 300.5f);
    EXPECT_FLOAT_EQ(loaded.player.health, 150.f); // integer literal accepted for a float setting
    EXPECT_FLOAT_EQ(loaded.enemy.damage, 20.f);
    EXPECT_EQ(loaded.waves.baseEnemies, 9);
    EXPECT_FLOAT_EQ(loaded.waves.delay, 0.5f);
    EXPECT_FLOAT_EQ(loaded.world.width, 3000.f);
}

TEST(App_GameTuning_Test, untouchedKeysKeepDefaults) {
    const Application::GameTuning defaults;
    const auto loaded = load("[Gameplay.Player]\nspeed = 1.0\n");

    EXPECT_FLOAT_EQ(loaded.player.speed, 1.f);
    EXPECT_FLOAT_EQ(loaded.player.damage, defaults.player.damage);
    EXPECT_FLOAT_EQ(loaded.enemy.radius, defaults.enemy.radius);
}

TEST(App_GameTuning_Test, wrongTypesFallBackToDefaults) {
    const Application::GameTuning defaults;
    const auto loaded = load(R"(
        [Gameplay.Player]
        speed = "fast"
        damage = true
        [Gameplay.Waves]
        base_enemies = "many"
    )");

    EXPECT_FLOAT_EQ(loaded.player.speed, defaults.player.speed);
    EXPECT_FLOAT_EQ(loaded.player.damage, defaults.player.damage);
    EXPECT_EQ(loaded.waves.baseEnemies, defaults.waves.baseEnemies);
}

TEST(App_GameTuning_Test, unknownKeysAndTablesAreIgnored) {
    const Application::GameTuning defaults;
    const auto loaded = load(R"(
        [Gameplay]
        not_a_setting = 5
        [Gameplay.Dragons]
        count = 3
    )");

    EXPECT_FLOAT_EQ(loaded.player.speed, defaults.player.speed);
}

TEST(App_GameTuning_Test, outOfRangeValuesAreClamped) {
    const auto loaded = load(R"(
        [Gameplay]
        knockback_decay = -5.0
        [Gameplay.Player]
        radius = 0.0
        attack_cooldown = -1.0
        health = -50.0
        [Gameplay.Waves]
        base_enemies = 0
        enemies_per_wave = -3
        [Gameplay.World]
        width = 1.0
    )");

    EXPECT_GE(loaded.knockbackDecay, 0.f);
    EXPECT_GT(loaded.player.radius, 0.f);
    EXPECT_GE(loaded.player.attackCooldown, 0.f);
    EXPECT_GT(loaded.player.health, 0.f);
    EXPECT_GE(loaded.waves.baseEnemies, 1);
    EXPECT_GE(loaded.waves.enemiesPerWave, 0);
    EXPECT_GE(loaded.world.width, 400.f);
}

TEST(App_GameTuning_Test, maxSpeedNeverBelowBaseSpeed) {
    const auto loaded = load(R"(
        [Gameplay.Enemy]
        base_speed = 200.0
        max_speed = 50.0
    )");

    EXPECT_GE(loaded.enemy.maxSpeed, loaded.enemy.baseSpeed);
}

// The shipped configs/gameplay.toml must describe exactly the built-in defaults, so that editing
// the file is the only thing that changes balance
TEST(App_GameTuning_Test, shippedConfigMatchesBuiltInDefaults) {
    const Application::GameTuning defaults;
    Core::TOMLConfigSection root(
        toml::parse_file(std::string(GAME_DIR) + "/configs/gameplay.toml"));
    auto gameplay = root.getSection(Application::GameTuning::SECTION_NAME);
    ASSERT_NE(gameplay, nullptr);
    const auto loaded = Application::GameTuning::fromConfig(gameplay.get());

    EXPECT_FLOAT_EQ(loaded.knockbackDecay, defaults.knockbackDecay);
    EXPECT_FLOAT_EQ(loaded.world.width, defaults.world.width);
    EXPECT_FLOAT_EQ(loaded.world.height, defaults.world.height);
    EXPECT_FLOAT_EQ(loaded.player.speed, defaults.player.speed);
    EXPECT_FLOAT_EQ(loaded.player.radius, defaults.player.radius);
    EXPECT_FLOAT_EQ(loaded.player.health, defaults.player.health);
    EXPECT_FLOAT_EQ(loaded.player.damage, defaults.player.damage);
    EXPECT_FLOAT_EQ(loaded.player.attackCooldown, defaults.player.attackCooldown);
    EXPECT_FLOAT_EQ(loaded.player.attackReach, defaults.player.attackReach);
    EXPECT_FLOAT_EQ(loaded.player.attackRadius, defaults.player.attackRadius);
    EXPECT_FLOAT_EQ(loaded.player.knockback, defaults.player.knockback);
    EXPECT_FLOAT_EQ(loaded.player.invulnerable, defaults.player.invulnerable);
    EXPECT_FLOAT_EQ(loaded.enemy.radius, defaults.enemy.radius);
    EXPECT_FLOAT_EQ(loaded.enemy.baseHealth, defaults.enemy.baseHealth);
    EXPECT_FLOAT_EQ(loaded.enemy.healthPerWave, defaults.enemy.healthPerWave);
    EXPECT_FLOAT_EQ(loaded.enemy.baseSpeed, defaults.enemy.baseSpeed);
    EXPECT_FLOAT_EQ(loaded.enemy.speedPerWave, defaults.enemy.speedPerWave);
    EXPECT_FLOAT_EQ(loaded.enemy.maxSpeed, defaults.enemy.maxSpeed);
    EXPECT_FLOAT_EQ(loaded.enemy.damage, defaults.enemy.damage);
    EXPECT_FLOAT_EQ(loaded.enemy.attackCooldown, defaults.enemy.attackCooldown);
    EXPECT_FLOAT_EQ(loaded.enemy.firstAttackDelay, defaults.enemy.firstAttackDelay);
    EXPECT_FLOAT_EQ(loaded.enemy.attackReach, defaults.enemy.attackReach);
    EXPECT_FLOAT_EQ(loaded.enemy.attackRadius, defaults.enemy.attackRadius);
    EXPECT_FLOAT_EQ(loaded.enemy.attackDistance, defaults.enemy.attackDistance);
    EXPECT_FLOAT_EQ(loaded.enemy.knockback, defaults.enemy.knockback);
    EXPECT_FLOAT_EQ(loaded.enemy.invulnerable, defaults.enemy.invulnerable);
    EXPECT_EQ(loaded.waves.baseEnemies, defaults.waves.baseEnemies);
    EXPECT_EQ(loaded.waves.enemiesPerWave, defaults.waves.enemiesPerWave);
    EXPECT_FLOAT_EQ(loaded.waves.delay, defaults.waves.delay);
    EXPECT_FLOAT_EQ(loaded.waves.firstDelay, defaults.waves.firstDelay);
    EXPECT_FLOAT_EQ(loaded.waves.minSpawnDistance, defaults.waves.minSpawnDistance);
}

// Full path used by the game: root.toml -> Configuration -> getSection("Gameplay") -> GameTuning
TEST(App_GameTuning_Test, loadsThroughRealConfigurationFromRootFile) {
    Core::ConfigSpec spec;
    spec.rootDirectory = GAME_DIR;
    spec.configDirectory = "configs";
    spec.rootConfigFile = "root.toml";
    spec.configReader = std::make_unique<Core::TOMLConfigReader>();

    Core::Configuration configuration(std::move(spec));
    const Core::ConfigSection* gameplay =
        configuration.getSection(Application::GameTuning::SECTION_NAME);

    ASSERT_NE(gameplay, nullptr) << "gameplay.toml is not listed in configs/root.toml";
    const auto loaded = Application::GameTuning::fromConfig(gameplay);
    EXPECT_FLOAT_EQ(loaded.player.speed, Application::GameTuning().player.speed);
}
