#include "application/inc/PlayerAction.hpp"


const std::string Actions::playerActionsEnumToString(const Actions::Player identifier)
{
    std::stringstream sStream;

    switch(identifier)
    {
        case Actions::Player::NONE:
        {
            sStream << "None";
            break;
        }
        case Actions::Player::MOVE_LEFT:
        {
            sStream << "Move Left";
            break;
        }
        case Actions::Player::MOVE_RIGHT:
        {
            sStream << "Move Right";
            break;
        }
        case Actions::Player::MOVE_DOWN:
        {
            sStream << "Move Down";
            break;
        }
        case Actions::Player::MOVE_UP:
        {
            sStream << "Move up";
            break;
        }
        case Actions::Player::JUMP:
        {
            sStream << "Jump";
            break;
        }
    }

    return sStream.str();
}