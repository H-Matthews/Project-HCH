#pragma once

#include "core/Messaging/MessageNetwork.hpp"
#include "core/Messaging/MessageTypes.hpp"

/**
 * The game's message IDs. Core only knows the generic Core::MessageID; adding a message to the
 * game means adding an ID here (and a name in registerMessageNames for readable logs).
 */
namespace Messages {
enum ID : Core::MessageID {
    NONE = Core::NO_MESSAGE,
    PlayerActionMessage = 1,
    EnemySpawnMessage = 2,
};

// Gives the IDs readable names in the MessageNetwork's log output
inline void registerMessageNames(Core::MessageNetwork& network) {
    network.registerMessageName(PlayerActionMessage, "PlayerAction Message");
    network.registerMessageName(EnemySpawnMessage, "EnemySpawn Message");
}
} // namespace Messages
