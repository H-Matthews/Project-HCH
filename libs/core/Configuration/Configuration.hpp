#pragma once

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include "core/Configuration/ConfigurationI.hpp"
#include "core/Configuration/ConfigFileID.hpp"

#include <string>
#include <functional>
#include <map>
#include <vector>

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

        bool configure() override;

        ~Configuration()
        {}

      private:
        bool initializeConfigFileIDs();

        std::pair< bool, std::string > parseRootFile();
        std::pair< bool, std::string > parseConfigFiles();

        std::pair< bool, std::string > handleCoreConfigurables( const std::string& filePath );
        std::pair< bool, std::string > handlePrefabs( const std::string& filePath );

        void initializeGlobalLogger();

      private:
        // Generic ConfigFileIDs
        std::vector< ConfigFileID > mConfigFileIDs;

        // string is the ConfigFileID
        std::map< ConfigFileID, std::pair< std::string, std::filesystem::path > > mConfigFiles;

        static const std::string ROOT_CONFIG_FILE_NAME;

      private:
        // FilePath information
        static const std::string CONFIG_DIR_NAME;
        static const std::string OUTPUT_DIR_NAME;
        static const std::string ASSET_DIR_NAME;
        static const std::string ASSET_FONTS_DIR_NAME;
        static const std::string ASSET_TEXTURES_DIR_NAME;
    };
}