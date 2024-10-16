#include "utility/inc/StringOperations.hpp"

const std::string Utility::statesEnumToString(Core::States::ID identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
        case Core::States::ID::NONE:
        {
            sStream << "None";
            break;
        }
        case Core::States::ID::Menu:
        {
            sStream << "Menu State";
            break;
        }
        case Core::States::ID::Game:
        {
            sStream << "Game State";
            break;
        }
        case Core::States::ID::Pause:
        {
            sStream << "Pause State";
            break;
        }
    }

    return sStream.str();
}

const std::string Utility::messageIDEnumToString(const Core::Messages::ID identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
        case Core::Messages::ID::NONE:
        {
            sStream << "NONE";
            break;
        }
        case Core::Messages::ID::PlayerActionMessage:
        {
            sStream << "PlayerAction Message";
            break;
        }
        case Core::Messages::ID::EnemySpawnMessage:
        {
            sStream << "EnemySpawn Message";
            break;
        }
    }

    return sStream.str();
}