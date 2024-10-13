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

const std::string Utility::messageIDEnumToString(const Message::ID identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
        case Message::ID::NONE:
        {
            sStream << "NONE";
            break;
        }
        case Message::ID::PlayerActionMessage:
        {
            sStream << "PlayerAction Message";
            break;
        }
        case Message::ID::EnemySpawnMessage:
        {
            sStream << "EnemySpawn Message";
            break;
        }
    }

    return sStream.str();
}