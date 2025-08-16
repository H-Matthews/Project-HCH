#pragma once

#include "core/Exceptions/ConfigurationException.hpp"
#include "core/Configuration/Configurables/ConfigInitializer.hpp"
#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include <string>
#include <cmath>
#include <functional>
#include <map>
#include <vector>
#include <filesystem>

namespace Core
{

    enum DirectoryIDs
    {
        OUTPUT = 0,
        CONFIG,
        ASSETS,
        SIZE
    };

    /**
     * Configuration sets up the Config Directory, Output Directory, Game asset file paths
     * Utilizes a TOML parser
     */
    class Configuration
    {
      public:
        Configuration( std::unique_ptr< ConfigReader > configReader, const std::string& configDirectoryName );

        void initializeOutputDirectory();
        void initializeAssetsDirectory();

        void parse();

        inline void setDirectoryInit( DirectoryIDs directoryID );

        inline bool isInitialized();

        inline bool isConfigInitialized();

        ~Configuration()
        {}

      private:
        std::pair< bool, std::string > parseRootFile();
        std::pair< bool, std::string > parseConfigFiles();

        std::pair< bool, std::filesystem::path > buildConfigFilePath( const std::string& configFile );

        void initializeGlobalLogger();

      private:
        std::unique_ptr< ConfigReader > mConfigReader;

        // Root Config File
        static const std::string ROOT_CONFIG_FILE_NAME;

        std::vector< std::string > mConfigFiles;

        // Default Filepath information
        static const std::string DEFAULT_OUTPUT_DIR_NAME;
        static const std::string DEFAULT_ASSET_DIR_NAME;
        static const std::string DEFAULT_ASSET_FONTS_DIR_NAME;
        static const std::string DEFAULT_ASSET_TEXTURES_DIR_NAME;

      public:
        std::string mProjectDirectory;

        std::string mConfigDirPath;
        std::string mOutputDirPath;

        std::string mAssetDirPath;
        std::string mAssetFontsDirPath;
        std::string mAssetTexturesDirPath;

      private:
        unsigned int mDirectoryBits : 3;
    };

    void Configuration::setDirectoryInit( DirectoryIDs directoryID )
    {
        mDirectoryBits = mDirectoryBits | 1 << directoryID;

        return;
    }

    bool Configuration::isInitialized()
    {
        // 7 is from 2^3 - 1
        return mDirectoryBits == ( std::pow( 2, (float)DirectoryIDs::SIZE ) ) - 1;
    }

    bool Configuration::isConfigInitialized()
    {
        // 2 is the ENUM value for CONFIG
        return mDirectoryBits == std::pow( 2, (float)DirectoryIDs::CONFIG );
    }
}