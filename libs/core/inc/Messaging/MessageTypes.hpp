#pragma once

#include <string>
#include <sstream>

namespace Messages
{
    enum class ID
    {
        NONE = 0,
        PlayerActionMessage,
        EnemySpawnMessage
    };

    const std::string messageIDEnumToString(const Messages::ID identifier);
}