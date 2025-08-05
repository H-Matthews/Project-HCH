#pragma once

#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include "vendor/toml/include/toml.hpp"

namespace Core
{
    class ConfigNode;

    /**
     * ConfigReader that parses the TOML syntax using the tomlplusplus library
     */
    class TOMLConfigReader : public ConfigReader
    {
      public:
        void init() override;

        std::pair< bool, std::string > readFile(
            const std::filesystem::path& filePath, std::shared_ptr< ConfigNode >& configNode ) override;

      private:
        void processTOMLData( const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode );

        void processPrimitiveTOMLData(
            const std::string& keyNode, const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode );

        void processArrayTOMLData( const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode );

      private:
        static const std::string PARAM_FILE_TYPE;
    };

}