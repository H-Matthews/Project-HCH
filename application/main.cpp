#include "core/Engine.hpp"

#include "core/Configuration/Configuration.hpp"
#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include "core/Exceptions/ConfigurationException.hpp"

#include <iostream>

constexpr const char* CONFIG_DIR_NAME = "configs";

int main()
{

    std::shared_ptr< Core::Engine > gameEngine = nullptr;

    try
    {
        auto gameConfig =
            std::make_unique< Core::Configuration >( std::make_unique< Core::TOMLConfigReader >(), CONFIG_DIR_NAME );

        gameConfig->parse();
        gameConfig->initializeOutputDirectory();
        gameConfig->initializeAssetsDirectory();

        auto gameEngine = Core::ConfigurableFactory::createTypedConfigurable< Core::Engine >( Core::Engine::TYPE_NAME );

        gameEngine->setConfiguration( std::move( gameConfig ) );
        gameEngine->initialize();
    }
    catch ( const Core::ConfigurationException& e )
    {
        std::cerr << e.what() << '\n';

        return -1;
    }

    if ( gameEngine )
        gameEngine->run();

    return 0;
}