#include "application/Scene/ECS/EntityManager.hpp"
#include "application/Scene/ECS/Entity.hpp"
#include "application/Scene/ECS/EntityFactory.hpp"

Application::Entity Application::EntityManager::createEntity() {
    Entity entity(mEntityRegistry.create(), this);

    return entity;
}

Application::Entity Application::EntityManager::createPlayerEntity(sf::Vector2f initialPosition) {
    return Entity(EntityFactory::createPlayer(mEntityRegistry, initialPosition), this);
}

Application::Entity Application::EntityManager::createEnemyEntity(sf::Vector2f initialPosition,
                                                                  int wave) {
    return Entity(EntityFactory::createEnemy(mEntityRegistry, initialPosition, wave), this);
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
