#include "core/Application.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/GameState.hpp"
#include "application/StateStack/PauseState.hpp"

#include <iostream>

constexpr const char* CONFIG_DIR_NAME = "configs";
constexpr const char* ROOT_FILE_NAME = "root.toml";

int main()
{
    Core::ConfigSpec configSpecification;
    configSpecification.configDirectory = CONFIG_DIR_NAME;
    configSpecification.rootConfigFile = ROOT_FILE_NAME;
    configSpecification.configReader = new Core::TOMLConfigReader();

    try
    {
        // TODO: Change this to an App Spec Type that
        // contains the Config Spec
        Core::Application application( configSpecification );

        application.initialize();
        application.pushState< Application::MenuState >();
        application.run();
    }
    catch (const Core::ConfigurationException& e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    return 0;
}