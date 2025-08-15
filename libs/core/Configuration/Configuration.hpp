#pragma once

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include "core/Configuration/ConfigurationI.hpp"
#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <filesystem>

namespace Core
{

    /**
     * Configuration sets up the Config Directory, Output Directory, Game asset file paths
     * Utilizes a TOML parser
     */
    class Configuration : public ConfigurationI
    {
      public:
        Configuration();

        void initializeOutputDirectory() override;
        void initializeConfigDirectory() override;
        void initializeAssetsDirectory() override;

        bool parse() override;

        ~Configuration()
        {}

      private:
        void registerConfigurables();

        std::pair< bool, std::string > parseRootFile();
        std::pair< bool, std::string > parseConfigFiles();

        std::pair< bool, std::filesystem::path > buildConfigFilePath( const std::string& configFile );

        void initializeGlobalLogger();

      private:
        std::unique_ptr< ConfigReader > mConfigReader;

      private:
        // Root Config File
        static const std::string ROOT_CONFIG_FILE_NAME;

        std::vector< std::string > mConfigFiles;

        // FilePath information
        static const std::string CONFIG_DIR_NAME;
        static const std::string OUTPUT_DIR_NAME;
        static const std::string ASSET_DIR_NAME;
        static const std::string ASSET_FONTS_DIR_NAME;
        static const std::string ASSET_TEXTURES_DIR_NAME;
    };
}