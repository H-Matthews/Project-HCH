#include "application/Scene/ECS/Systems/MovementSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

#include <algorithm>
#include <cmath>

void System::MovementSystem::update(const sf::Time deltaTime) {
    const float dt = deltaTime.asSeconds();
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    const float knockbackFade = std::exp(-tuning.knockbackDecay * dt);

    auto view = mRegistry->view<Component::Transform, Component::Velocity>();

    for (auto entity : view) {
        auto& transform = view.get<Component::Transform>(entity);
        auto& velocity = view.get<Component::Velocity>(entity);

        transform.position += (velocity.move + velocity.knockback) * dt;
        velocity.knockback *= knockbackFade;

        float margin = 0.f;
        if (const auto* collider = mRegistry->try_get<Component::Collider>(entity)) {
            margin = collider->radius;
        }

        transform.position.x =
            std::clamp(transform.position.x, margin, tuning.world.width - margin);
        transform.position.y =
            std::clamp(transform.position.y, margin, tuning.world.height - margin);
    }
}
