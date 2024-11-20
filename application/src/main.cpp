#include "application/inc/Application.hpp"

#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/Logger.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>

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