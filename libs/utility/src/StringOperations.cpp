#include "utility/inc/StringOperations.hpp"
namespace Utility 
{
    const std::string statesEnumToString(States::ID identifier)
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

    const std::string messageIDEnumToString(const Messages::ID identifier)
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

    const std::string logLevelEnumToString(const LogLevel identifier)
    {
        /*
         DEBUG = 0,
        INFO,
        WARNING,
        ERROR
        */

        std::stringstream sStream;

        switch(identifier)
        {
            case LogLevel::NONE:
            {
                sStream << "NONE";
                break;
            }
            case LogLevel::DEBUG:
            {
                sStream << "DEBUG";
                break;
            }
            case LogLevel::INFO:
            {
                sStream << "INFO";
                break;
            }
            case LogLevel::WARN:
            {
                sStream << "WARN";
                break;
            }
            case LogLevel::ERROR:
            {
                sStream << "ERROR";
                break;
            }
        }

        return sStream.str();
    }
}