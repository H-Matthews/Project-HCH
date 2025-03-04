#pragma once

#include <string>

namespace Messages
{
    /**
     * Defines application specific Message IDs
     */
    enum class ID
    {
        NONE = 0,
        PlayerActionMessage,
        EnemySpawnMessage
    };

    const std::string messageIDEnumToString( const Messages::ID identifier );
}