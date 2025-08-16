#include "core/Configuration/Configurables/ConfigInitializer.hpp"

// Core Configurables
#include "core/Engine.hpp"
#include "core/StateStack/StateStack.hpp"
#include "core/Messaging/MessageNetwork.hpp"
#include "application/Scene/Scene.hpp"

void Core::ConfigInitializer::registerCoreConfigurables()
{
    // Register Configurables
    ConfigurableFactory::registerType( Core::Engine::TYPE_NAME, []() { return std::make_shared< Core::Engine >(); } );

    ConfigurableFactory::registerType(
        Core::StateStack::TYPE_NAME, []() { return std::make_shared< Core::StateStack >(); } );

    ConfigurableFactory::registerType(
        Core::MessageNetwork::TYPE_NAME, []() { return std::make_shared< Core::MessageNetwork >(); } );

    // ConfigurableFactory::registerType(
    //     Application::Scene::TYPE_NAME, []() { return std::make_unique< Application::Scene >(); } );
}