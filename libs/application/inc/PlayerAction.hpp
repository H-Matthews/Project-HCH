#pragma once

#include <string>
#include <sstream>

/**
 * Represents Player Actions
 */

namespace Actions
{
    enum class Player
    {
        NONE = 0,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_DOWN,
        MOVE_UP,
        JUMP,
    };

    const std::string playerActionsEnumToString( const Actions::Player identifier );
}