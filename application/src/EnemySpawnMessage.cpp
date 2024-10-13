#include "EnemySpawnMessage.hpp"

EnemySpawnMessage::EnemySpawnMessage(const Message::ID messageID ) :
    Message( messageID, "EnemySpawnMessage"),
    spawn()
{
}