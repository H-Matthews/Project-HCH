#include "utility/inc/Logger.hpp"

#include <iostream>

void Utility::Logger::log(const std::string& message)
{
    std::cout << message << std::endl;
}