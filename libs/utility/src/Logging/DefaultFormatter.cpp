#include "utility/inc/Logging/DefaultFormatter.hpp"

#include <iomanip>
#include <iostream>
#include <chrono>

Utility::DeafaultFormatter::DeafaultFormatter() :
    FormatterI("KeyValue")
{
}

std::string Utility::DeafaultFormatter::format(std::string message, LogLevel level, const std::source_location location)
{
    
}