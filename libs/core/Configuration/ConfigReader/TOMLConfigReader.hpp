#pragma once

#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include "core/Configuration/ConfigReader/ConfigType/RootConfigType.hpp"

#include "vendor/toml/include/toml.hpp"

namespace Core
{
    /**
     * ConfigReader that parses the TOML syntax using the tomlplusplus library
     */
    class TOMLConfigReader : public ConfigReader
    {
      public:
        void init() override;

        void readFile( const std::filesystem::path& filePath, ConfigNode& configNode ) override;

      private:
        std::pair< bool, std::string > handleRootFile(
            const toml::table& configTable, std::shared_ptr< RootConfigType > rootConfigType );

        // std::pair< bool, std::string > handleCoreConfigFile(
        //     const toml::table& configTable, std::shared_ptr< ConfigNode > configNode );

        std::pair< bool, std::string > extractTOMLString(
            const toml::table& configTable, const std::string& stringParameter, std::string& extractedString );

      private:
        static const std::string PARAM_FILE_TYPE;
    };

}