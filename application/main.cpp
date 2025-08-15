#include "application/App.hpp"

#include "core/Configuration/Configurables/ConfigInitializer.hpp"
#include "core/Configuration/Configuration.hpp"

#include <iostream>
#include <stdexcept>

int main()
{
    Core::ConfigInitializer::registerConfigurables();
    auto application =
        Core::ConfigurableFactory::createTypedConfigurable< Application::App >( Application::App::TYPE_NAME );

    auto gameConfig = std::make_unique< Core::Configuration >();

    application->setConfiguration( std::move( gameConfig ) );
    application->initialize();
    application->run();

    try
    {
        // game.initialize();
        // game.run();
    }
    catch ( const std::exception& e )
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}