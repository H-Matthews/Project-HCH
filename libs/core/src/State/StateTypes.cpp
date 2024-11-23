#include "core/inc/State/StateTypes.hpp"


const std::string States::statesEnumToString(const States::ID identifier)
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
            sStream << "Menu";
            break;
        }
        case States::ID::Game:
        {
            sStream << "Game";
            break;
        }
        case States::ID::Pause:
        {
            sStream << "Pause";
            break;
        }
    }

    return sStream.str();
}