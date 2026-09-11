#include "application/Scene/ECS/Components/PlayerInputComponent.hpp"

Component::PlayerInputComponent::PlayerInputComponent(sf::Vector2f playerVelocity)
    : mPlayerVelocity(playerVelocity) {}
