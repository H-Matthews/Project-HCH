#pragma once

#include <string>

namespace Core
{

    /**
     * Each ID represents a Configuration File
     */
    enum class ConfigFileID
    {
        CORE_CONFIGURABLES = 0,
        PREFABS,
        SIZE
    };

    ConfigFileID stringToEnum( const std::string& stringEnum );
    std::string enumToString( const ConfigFileID& configID );
}