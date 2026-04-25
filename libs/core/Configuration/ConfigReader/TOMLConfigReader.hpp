#pragma once

#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include "vendor/toml/include/toml.hpp"

namespace Core
{
    class TOMLConfigReader : public ConfigReader
    {
      public:
        std::unique_ptr<ConfigSection> readFile(const std::filesystem::path& filePath) override;
    };
}
