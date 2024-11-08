#include "utility/inc/StringOperations.hpp"

const std::string Utility::statesEnumToString(States::ID identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
        case States::ID::NONE:
        {
            sStream << "None";
            break;
        }
        case States::ID::Menu:
        {
            sStream << "Menu State";
            break;
        }
        case States::ID::Game:
        {
            sStream << "Game State";
            break;
        }
        case States::ID::Pause:
        {
            sStream << "Pause State";
            break;
        }
    }

    return sStream.str();
}

const std::string Utility::messageIDEnumToString(const Messages::ID identifier)
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