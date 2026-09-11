#include "core/Application.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

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

    Core::ConfigSpec configSpecification;
    configSpecification.configDirectory = CONFIG_DIR_NAME;
    configSpecification.rootConfigFile = ROOT_FILE_NAME;
    configSpecification.configReader = std::make_unique<Core::TOMLConfigReader>();

    try {
        // TODO: Change this to an App Spec Type that
        // contains the Config Spec
        Core::Application application(std::move(configSpecification));

        application.initialize();
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
