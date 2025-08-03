#pragma once

#include "core/Configuration/ConfigReader/Nodes/ConfigNode.hpp"

#include <filesystem>
#include <string>

namespace Core
{
    /**
     * Interface class for ALL config readers
     */
    class ConfigReader
    {
      public:
        virtual void init() = 0;

        virtual void readFile( const std::filesystem::path& filePath, ConfigNode& configNode ) = 0;
    };
}