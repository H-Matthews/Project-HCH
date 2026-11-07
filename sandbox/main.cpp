#include "core/Application.hpp"

#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"
#include "core/Configuration/Configuration.hpp"
#include "core/StateStack/State.hpp"
#include "core/Window/SfmlWindow.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Logger.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

#include <iostream>
#include <string_view>

/**
 * Sandbox: a minimal executable that links only the engine. Use it to try out engine features
 * without any game code. Press Escape to quit.
 */
class SandboxState : public Core::State {
  public:
    static constexpr std::string_view TYPE = "SandboxState";

    SandboxState(Core::StateStack& stack, Core::Context ctx) : State(stack, ctx) {}

    std::string_view getStateType() override { return TYPE; }

    void handleEvent(const Core::Event& event) override {
        if (auto* e = std::get_if<Core::KeyPressed>(&event)) {
            if (e->key == Core::Key::Escape) {
                requestStateClear();
            }
        }
    }

    void update(sf::Time) override {
        if (input().isDown(Core::Key::W)) {
            std::cout << "W Key" << std::endl;
        }
        if (input().isDown(Core::Key::S)) {
            std::cout << "S Key" << std::endl;
        }
        if (input().isDown(Core::Key::A)) {
            std::cout << "A Key" << std::endl;
        }
        if (input().isDown(Core::Key::D)) {
            std::cout << "D Key" << std::endl;
        }

        if (input().wasPressed(Core::MouseButton::Left)) {
            std::cout << "Clicked Left Mouse Button" << std::endl;
        }
        if (input().wasPressed(Core::MouseButton::Right)) {
            std::cout << "Clicked Right Mouse Button" << std::endl;
        }

        // if(input().isDown(Core::MouseButton::Left)) {
        //     std::cout << "Left Mouse Button" << std::endl;
        // }
        // if(input().isDown(Core::MouseButton::Right)) {
        //     std::cout << "Right Mouse Button" << std::endl;
        // }
    }

    void draw() override { renderWindow().clear(sf::Color(30, 30, 40)); }
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

    Core::ConfigSpec configSpec;
    configSpec.rootDirectory = SANDBOX_DIR;
    configSpec.configDirectory = "configs";
    configSpec.rootConfigFile = "root.toml";
    configSpec.configReader = std::make_unique<Core::TOMLConfigReader>();

    Core::AppSpec appSpec(std::move(configSpec));

    try {
        Core::Application application(std::move(appSpec));

        application.initialize();
        application.pushState<SandboxState>();
        application.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

        return -1;
    }

    return 0;
}
