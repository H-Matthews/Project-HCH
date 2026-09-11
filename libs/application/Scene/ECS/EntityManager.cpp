#include "application/Scene/ECS/EntityManager.hpp"
#include "application/Scene/ECS/Entity.hpp"
#include "application/Scene/ECS/Components/Components.hpp"

Application::EntityManager::EntityManager(TextureHolder& textures) : mTextures(textures) {}

Application::Entity Application::EntityManager::createEntity() {
    Entity entity(mEntityRegistry.create(), this);

    return entity;
}

Application::Entity Application::EntityManager::createPlayerEntity(sf::Vector2f initialPosition,
                                                                   sf::Vector2f initialVelocity) {
    Entity entity(mEntityRegistry.create(), this);

    // Grab TextureID
    sf::Texture& texture = mTextures.get(Textures::ID::PLAYER);
    sf::Sprite sprite(texture);

    entity.addComponent<Component::SpriteComponent>(sprite, initialPosition, initialVelocity);
    entity.addComponent<Component::PlayerInputComponent>(sf::Vector2f{0.f, 0.f});

    return entity;
}

void Application::EntityManager::registerSystem(std::unique_ptr<System::BaseSystem> system) {
    system->init(mEntityRegistry);
    systemsContainer.push_back(std::move(system));
}

void Application::EntityManager::registerRenderingSystem(
    std::unique_ptr<System::BaseSystem> renderSystem) {
    renderSystem->init(mEntityRegistry);
    renderingSystemsContainer.push_back(std::move(renderSystem));
}

void Application::EntityManager::update(sf::Time timeStep) {
    for (const auto& system : systemsContainer) {
        system->update(timeStep);
    }
}

void Application::EntityManager::render() {
    for (const auto& renderSystem : renderingSystemsContainer) {
        renderSystem->render();
    }
}