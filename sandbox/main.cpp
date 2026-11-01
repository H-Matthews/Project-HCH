#include "core/Application.hpp"

#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/StateStack/State.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Logger.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

#include <iostream>

/**
 * Sandbox: a minimal executable that links only the engine. Use it to try out engine features
 * without any game code. Press Escape to quit.
 */
class SandboxState : public Core::State {
  public:
    SandboxState() : State("SANDBOX") {}

    void draw() override { getApplication().getWindow().clear(sf::Color(30, 30, 40)); }

    bool update(sf::Time) override { return false; }

    bool handleKeyPressed(const sf::Event::KeyPressed& keyPressedEvent) override {
        if (keyPressedEvent.scancode == sf::Keyboard::Scancode::Escape) {
            requestStateClear();
        }

        return false;
    }
};

static void initializeGlobalLogger() {
    if constexpr (Utility::CAN_LOG) {
        Utility::createGlobalLogger();

        auto logger = Utility::LogRegistry::instance()->getGlobalLogger();
        if (logger) {
            logger->addSink(std::make_shared<Utility::ColorConsoleSink>());
        }
    }
}

int main() {
    initializeGlobalLogger();

    Core::ConfigSpec configSpecification;
    configSpecification.rootDirectory = SANDBOX_DIR;
    configSpecification.configDirectory = "configs";
    configSpecification.rootConfigFile = "root.toml";
    configSpecification.configReader = std::make_unique<Core::TOMLConfigReader>();

    try {
        Core::Application application(std::move(configSpecification));

        application.initialize();
        application.pushState<SandboxState>();
        application.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

        return -1;
    }

    return 0;
}
