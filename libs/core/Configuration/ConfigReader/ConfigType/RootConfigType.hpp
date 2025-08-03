#pragma once

#include "core/Configuration/ConfigReader/ConfigType/ConfigType.hpp"
#include "core/Configuration/ConfigFileID.hpp"

#include <vector>
#include <map>

namespace Core
{
    struct RootConfigType : public ConfigType
    {
        RootConfigType() :
            ConfigType( "RootConfigType" ),
            configFiles()
        {}

        std::vector< std::string > configFiles;

        const std::vector< std::string > absoluteTablePaths = { "Configuration_Files.CORE_CONFIGURABLES",
            "Configuration_Files.PREFABS" };
    };
}