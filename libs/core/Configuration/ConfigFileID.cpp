#include "core/Configuration/ConfigFileID.hpp"

#include <iostream>

Core::ConfigFileID Core::stringToEnum( const std::string& stringEnum )
{
    ConfigFileID fileID;

    if ( stringEnum == "CORE_CONFIGURABLES" )
    {
        fileID = ConfigFileID::CORE_CONFIGURABLES;
    }
    else if ( stringEnum == "PREFABS" )
    {
        fileID = ConfigFileID::PREFABS;
    }
    else
    {
        std::cout << "The string: " << stringEnum << " could not be correlated to a ConfigFileID! " << std::endl;
        std::cout << "The ID was probably not added to the stringToEnum function " << std::endl;
    }

    return fileID;
}

std::string Core::enumToString( const ConfigFileID& configID )
{
    std::string retString( "" );

    switch ( configID )
    {
        case ConfigFileID::CORE_CONFIGURABLES:
        {
            retString = "CORE_CONFIGURABLES";

            break;
        }
        case ConfigFileID::PREFABS:
        {
            retString = "PREFABS";

            break;
        }
        case ConfigFileID::SIZE:
        {
            break;
        }
    }

    return retString;
}