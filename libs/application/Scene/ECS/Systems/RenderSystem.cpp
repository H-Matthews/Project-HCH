#include "application/Scene/ECS/Systems/RenderSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>

System::RenderSystem::RenderSystem(sf::RenderWindow& window) : mWindow(window) {}

void System::RenderSystem::render() {
    updateCamera();
    drawArena();
    drawShapes();
    drawHealthBars();
}

void System::RenderSystem::updateCamera() {
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    const sf::Vector2f size(mWindow.getSize());
    sf::Vector2f center = tuning.worldSize() / 2.f;

    for (auto entity : mRegistry->view<Component::Player, Component::Transform>())
        center = mRegistry->get<Component::Transform>(entity).position;

    // Keep the view inside the arena (when the arena is larger than the window)
    if (tuning.world.width > size.x)
        center.x = std::clamp(center.x, size.x / 2.f, tuning.world.width - size.x / 2.f);
    else
        center.x = tuning.world.width / 2.f;

    if (tuning.world.height > size.y)
        center.y = std::clamp(center.y, size.y / 2.f, tuning.world.height - size.y / 2.f);
    else
        center.y = tuning.world.height / 2.f;

    mWindow.setView(sf::View(center, size));
}

void System::RenderSystem::drawArena() {
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    sf::RectangleShape floor(tuning.worldSize());
    floor.setFillColor(sf::Color(34, 38, 48));
    floor.setOutlineColor(sf::Color(120, 90, 70));
    floor.setOutlineThickness(-8.f);
    mWindow.draw(floor);

    constexpr float GRID = 100.f;
    const sf::Color gridColor(44, 49, 62);
    sf::VertexArray lines(sf::PrimitiveType::Lines);

    for (float x = GRID; x < tuning.world.width; x += GRID) {
        lines.append(sf::Vertex{{x, 0.f}, gridColor});
        lines.append(sf::Vertex{{x, tuning.world.height}, gridColor});
    }
    for (float y = GRID; y < tuning.world.height; y += GRID) {
        lines.append(sf::Vertex{{0.f, y}, gridColor});
        lines.append(sf::Vertex{{tuning.world.width, y}, gridColor});
    }

    mWindow.draw(lines);
}

void System::RenderSystem::drawShapes() {
    auto view = mRegistry->view<Component::Shape, Component::Transform>();

    sf::CircleShape circle;
    circle.setPointCount(28);

    // Cheap layer ordering: a pass per layer
    for (int layer = 0; layer <= 3; ++layer) {
        for (auto entity : view) {
            const auto& shape = view.get<Component::Shape>(entity);
            if (shape.layer != layer)
                continue;

            const auto& transform = view.get<Component::Transform>(entity);

            sf::Color color = shape.color;
            const auto* health = mRegistry->try_get<Component::Health>(entity);
            if (health && health->invulnerableTimer > 0.f)
                color = sf::Color::White; // Hit flash

            circle.setRadius(shape.radius);
            circle.setOrigin({shape.radius, shape.radius});
            circle.setPosition(transform.position);
            circle.setFillColor(color);
            mWindow.draw(circle);

            // Little dot showing which way a character is facing
            if (mRegistry->all_of<Component::MeleeAttacker>(entity)) {
                sf::CircleShape nose(shape.radius * 0.3f);
                nose.setOrigin({nose.getRadius(), nose.getRadius()});
                nose.setPosition(transform.position + transform.facing * shape.radius * 0.7f);
                nose.setFillColor(sf::Color(20, 20, 30));
                mWindow.draw(nose);
            }
        }
    }
}

void System::RenderSystem::drawHealthBars() {
    auto view = mRegistry->view<Component::Health, Component::Transform, Component::Collider>();

    sf::RectangleShape bar;
    for (auto entity : view) {
        const auto& health = view.get<Component::Health>(entity);
        if (health.current >= health.max)
            continue; // Only show bars on damaged characters

        const auto& transform = view.get<Component::Transform>(entity);
        const float radius = view.get<Component::Collider>(entity).radius;
        const float width = radius * 2.f;
        const sf::Vector2f topLeft(transform.position.x - radius,
                                   transform.position.y - radius - 12.f);

        bar.setSize({width, 5.f});
        bar.setPosition(topLeft);
        bar.setFillColor(sf::Color(60, 20, 20));
        mWindow.draw(bar);

        bar.setSize({width * std::clamp(health.current / health.max, 0.f, 1.f), 5.f});
        bar.setFillColor(sf::Color(90, 220, 90));
        mWindow.draw(bar);
    }
}
