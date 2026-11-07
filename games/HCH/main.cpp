#include "core/Application.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include "application/Data/GameResources.hpp"
#include "application/Scene/ECS/GameTuning.hpp"
#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/GameState.hpp"
#include "application/StateStack/PauseState.hpp"

#include "utility/Logging/Logger.hpp"
#include "utility/Logging/LogRegistry.hpp"
#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

#include <iostream>

constexpr const char* CONFIG_DIR_NAME = "configs";
constexpr const char* ROOT_FILE_NAME = "root.toml";

static void initializeGlobalLogger() {
    if constexpr (Utility::CAN_LOG) {
        Utility::createGlobalLogger();

        auto cLogger = Utility::LogRegistry::instance()->getGlobalLogger();
        if (cLogger) {
            cLogger->addSink(std::make_shared<Utility::ColorConsoleSink>());
            cLogger->logInfo("Initialized Global Logger");
        }
    }
}

int main() {
    initializeGlobalLogger();

    Core::ConfigSpec configSpec;
    configSpec.rootDirectory = GAME_DIR;
    configSpec.configDirectory = CONFIG_DIR_NAME;
    configSpec.rootConfigFile = ROOT_FILE_NAME;
    configSpec.configReader = std::make_unique<Core::TOMLConfigReader>();

    Core::AppSpec appSpec(std::move(configSpec));

    try {
        Core::Application application(std::move(appSpec));

        application.initialize();
        Application::loadGameResources(application);
        Application::GameState::setTuning(Application::GameTuning::fromConfig(
            application.getConfigSection(Application::GameTuning::SECTION_NAME)));

        application.pushState<Application::MenuState>();
        application.run();
    } catch (const Core::ConfigurationException& e) {
        std::cerr << e.what() << '\n';

        return -1;
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';

        return -1;
    }

    return 0;
}
