#include "core/ecs/EntityManager.hpp"
#include "core/ecs/Entity.hpp"

Core::Entity Core::EntityManager::createEntity() {
    Entity entity(mEntityRegistry.create(), this);

    return entity;
}

void Core::EntityManager::registerSystem(std::unique_ptr<BaseSystem> system) {
    system->init(mEntityRegistry);
    systemsContainer.push_back(std::move(system));
}

void Core::EntityManager::registerRenderingSystem(std::unique_ptr<BaseSystem> renderSystem) {
    renderSystem->init(mEntityRegistry);
    renderingSystemsContainer.push_back(std::move(renderSystem));
}

void Core::EntityManager::update(sf::Time timeStep) {
    for (const auto& system : systemsContainer) {
        system->update(timeStep);
    }
}

void Core::EntityManager::render() {
    for (const auto& renderSystem : renderingSystemsContainer) {
        renderSystem->render();
    }
}
