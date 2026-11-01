#include "application/Scene/ECS/Systems/PlayerInputSystem.hpp"

#include "application/Scene/ECS/Components/Components.hpp"
#include "utility/VectorMath.hpp"
#include "application/Scene/ECS/GameTuning.hpp"

System::PlayerInputSystem::PlayerInputSystem(Core::MessageNetwork& network)
    : Core::MessageNode(network, "PlayerInput System", Core::NodeType::SUBSCRIBER) {
    // Set Subscriber Topics
    MessageNode::addTopic(Messages::ID::PlayerActionMessage);
}

void System::PlayerInputSystem::onNotify(Core::Message* message) {
    auto* playerInput = dynamic_cast<Application::PlayerInputMessage*>(message);
    if (!playerInput || !mRegistry) {
        return;
    }

    auto view = mRegistry->view<Component::PlayerInputComponent>();

    for (auto entity : view) {
        auto& input = view.get<Component::PlayerInputComponent>(entity);

        switch (playerInput->action) {
        case Actions::Player::MOVE_UP:
            input.direction.y -= 1.f;
            break;
        case Actions::Player::MOVE_DOWN:
            input.direction.y += 1.f;
            break;
        case Actions::Player::MOVE_LEFT:
            input.direction.x -= 1.f;
            break;
        case Actions::Player::MOVE_RIGHT:
            input.direction.x += 1.f;
            break;
        case Actions::Player::ATTACK:
            input.attackRequested = true;
            break;
        case Actions::Player::NONE:
            break;
        }
    }
}

void System::PlayerInputSystem::update(const sf::Time) {
    const auto& tuning = mRegistry->ctx().get<Application::GameTuning>();
    auto view = mRegistry->view<Component::PlayerInputComponent, Component::Velocity,
                                Component::Transform, Component::MeleeAttacker>();

    for (auto entity : view) {
        auto& input = view.get<Component::PlayerInputComponent>(entity);
        auto& velocity = view.get<Component::Velocity>(entity);
        auto& transform = view.get<Component::Transform>(entity);
        auto& attacker = view.get<Component::MeleeAttacker>(entity);

        const sf::Vector2f direction = Utility::Math::normalized(input.direction);
        velocity.move = direction * tuning.player.speed;

        // Keep the previous facing while standing still
        if (direction.x != 0.f || direction.y != 0.f) {
            transform.facing = direction;
        }

        if (input.attackRequested) {
            attacker.wantsAttack = true;
        }

        input.direction = {0.f, 0.f};
        input.attackRequested = false;
    }
}
