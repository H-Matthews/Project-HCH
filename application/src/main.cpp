#include "Application.hpp"

#include <iostream>
#include <stdexcept>

int main()
{

    try
    {
        Application game;
        game.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    
    return 0;
}