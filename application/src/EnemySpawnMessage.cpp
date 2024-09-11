#include "EnemySpawnMessage.hpp"

EnemySpawnMessage::EnemySpawnMessage(const std::pair< Message::ID, std::string >& messageID ) :
    Message( messageID ),
    spawn()
{
}

EnemySpawnMessage::EnemySpawnMessage( EnemySpawnMessage* message)
{
    mMessageInfo.sender = message->mMessageInfo.sender;
    mMessageInfo.messageID = message->mMessageInfo.messageID;
    spawn = message->spawn;
}