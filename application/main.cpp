#include "core/Application.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include "application/StateStack/MenuState.hpp"
#include "application/StateStack/GameState.hpp"
#include "application/StateStack/PauseState.hpp"

#include <iostream>

constexpr const char* CONFIG_DIR_NAME = "configs";
constexpr const char* ROOT_FILE_NAME = "root.toml";

void registerGameStates( Core::Application& application );

int main()
{
    Core::ConfigSpec configSpecification;
    configSpecification.configDirectory = CONFIG_DIR_NAME;
    configSpecification.rootConfigFile = ROOT_FILE_NAME;
    configSpecification.configReader = new Core::TOMLConfigReader();

    try
    {
        // TODO: Change this to an Application Specification that
        // encapsulates the configSpecification
        Core::Application application( configSpecification );

        registerGameStates( application );

        application.pushState( Application::MenuState::identifier );

        application.initialize();
        application.run();
    }
    catch ( const Core::ConfigurationException& e )
    {
        std::cerr << e.what() << '\n';

        return -1;
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    return 0;
}

void registerGameStates( Core::Application& application )
{
    const std::string menuID = Application::MenuState::identifier;
    const std::string gameID = Application::GameState::identifier;
    const std::string pauseID = Application::PauseState::identifier;

    application.registerState( menuID, [ menuID ]() { return std::make_unique< Application::MenuState >( menuID ); } );
    application.registerState( gameID, [ gameID ]() { return std::make_unique< Application::GameState >( gameID ); } );

    application.registerState(
        pauseID, [ pauseID ]() { return std::make_unique< Application::PauseState >( pauseID ); } );

    return;
}