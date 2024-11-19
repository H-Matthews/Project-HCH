#include "application/inc/Application.hpp"

#include "core/inc/Configuration.hpp"

#include <iostream>
#include <stdexcept>

int main()
{

    try
    {
        auto config = std::make_shared<Core::Configuration>();

        Application game(config);
        game.initialize();
        game.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    
    return 0;
}