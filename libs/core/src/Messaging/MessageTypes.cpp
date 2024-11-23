#include "core/inc/Messaging/MessageTypes.hpp"


const std::string Messages::messageIDEnumToString(const Messages::ID identifier)
{
    std::stringstream sStream;

        switch(identifier)
        {
            case Messages::ID::NONE:
            {
                sStream << "None";
                break;
            }
            case Messages::ID::PlayerActionMessage:
            {
                sStream << "PlayerAction Message";
                break;
            }
            case Messages::ID::EnemySpawnMessage:
            {
                sStream << "EnemySpawn Message";
                break;
            }
        }

        return sStream.str();
}