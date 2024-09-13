#include "EnemySpawnMessage.hpp"

EnemySpawnMessage::EnemySpawnMessage(const Message::ID messageID ) :
    Message( messageID, "EnemySpawnMessage"),
    spawn()
{
}

std::unique_ptr<Message> EnemySpawnMessage::clone() const
{
    return std::make_unique<EnemySpawnMessage>(*this);
}