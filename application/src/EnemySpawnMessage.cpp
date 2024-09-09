#include "EnemySpawnMessage.hpp"

EnemySpawnMessage::EnemySpawnMessage(MessageNetwork& messageNetwork, std::string identifierString) :
    Message(messageNetwork, identifierString)
{
}

EnemySpawnMessage::EnemySpawnMessage()
{
}

EnemySpawnMessage::EnemySpawnMessage( EnemySpawnMessage* message)
{
    mSender = message->mSender;
    mIdentifierString = message->mIdentifierString;
    mIDType = message->mIDType;
}