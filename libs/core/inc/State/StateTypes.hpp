#pragma once

#include <sstream>

namespace States
{
    enum ID
    {
        NONE = 0,
        Menu,
        Game, 
        Pause
    };

    const std::string statesEnumToString(const States::ID identifier);
}