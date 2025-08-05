#pragma once

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

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

        virtual std::pair< bool, std::string > readFile(
            const std::filesystem::path& filePath, std::shared_ptr< ConfigNode >& configNode ) = 0;
    };
}