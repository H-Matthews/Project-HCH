#include "StringOperations.hpp"

const std::string Utility::statesEnumToString(States::ID identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
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
        default:
            sStream << "No State";
    }

    return sStream.str();
}

const std::string Utility::messageEnumToString(const Message::ID identifier)
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
            sStream << "PlayerActionMessage ";
            break;
        }
        default:
            sStream << "No Message";
    }

    return sStream.str();
}