#pragma once

#include <string>

namespace Core
{

    /**
     * Each ID represents a Configuration File
     */
    enum class ConfigFileID
    {
        ROOT = 0,
        CORE_CONFIGURABLES,
        PREFABS,
        SIZE
    };

    ConfigFileID stringToEnum( const std::string& stringEnum );
    std::string enumToString( const ConfigFileID& configID );
}