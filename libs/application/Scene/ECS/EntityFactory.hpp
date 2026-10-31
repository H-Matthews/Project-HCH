#pragma once

#include "vendor/entt/include/entt.hpp"

#include "application/Scene/ECS/Components/GameplayComponents.hpp"

#include <SFML/System/Vector2.hpp>

namespace Application::EntityFactory {

entt::entity createPlayer(entt::registry& registry, sf::Vector2f position);
entt::entity createEnemy(entt::registry& registry, sf::Vector2f position, int wave);
entt::entity createHitbox(entt::registry& registry, sf::Vector2f position, Component::TeamID team,
                          float damage, float knockback, float radius);

} // namespace Application::EntityFactory
