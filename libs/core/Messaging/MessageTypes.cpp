#include "core/Messaging/MessageTypes.hpp"

const std::string Messages::messageIDEnumToString(const Messages::ID identifier) {
    std::string string;

    switch (identifier) {
    case Messages::ID::NONE: {
        string = "None";
        break;
    }
    case Messages::ID::PlayerActionMessage: {
        string = "PlayerAction Message";
        break;
    }
    case Messages::ID::EnemySpawnMessage: {
        string = "EnemySpawn Message";
        break;
    }
    }

    return string;
}