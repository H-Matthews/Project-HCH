#include "application/inc/App.hpp"

#include "core/inc/Configuration/Configuration.hpp"

#include <iostream>
#include <stdexcept>

int main()
{

    try
    {
        Application::App game( std::make_unique< Core::Configuration >() );
        game.initialize();
        game.run();
    }
    catch ( const std::exception& e )
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}