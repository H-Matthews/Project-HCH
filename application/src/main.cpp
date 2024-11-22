#include "application/inc/Application.hpp"

#include "core/inc/Configuration.hpp"

// Logging Tests
#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <iostream>
#include <stdexcept>

int main()
{

    try
    {
        auto config = std::make_shared<Core::Configuration>();

        Application game(config);
        game.initialize();
        //game.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    
    return 0;
}