#pragma once

#include "core/Exceptions/ConfigurationException.hpp"
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
    struct ConfigSpec
    {
        std::string rootConfigFile;
        std::string configDirectory;

        ConfigReader* configReader;

        ConfigSpec();
        ConfigSpec( const ConfigSpec& other ) = default;
    };

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
        explicit Configuration( ConfigSpec configSpec );

        void setDirectoryInit( DirectoryIDs directoryID );

        bool isInitialized() const;

        bool isConfigInitialized() const;

        ~Configuration() = default;

      private:
        void configure();

        void initializeOutputDirectory();
        void initializeAssetsDirectory();

        void parse();
        std::pair< bool, std::string > parseRootFile();
        std::pair< bool, std::string > parseConfigFiles();

        std::pair< bool, std::filesystem::path > buildConfigFilePath( const std::string& configFile );

        void initializeGlobalLogger();

      public:
        std::string mProjectDirectory;

        std::string mRootFile;
        std::string mConfigDirectory;

        std::string mConfigDirPath;
        std::string mOutputDirPath;

        std::string mAssetDirPath;
        std::string mAssetFontsDirPath;
        std::string mAssetTexturesDirPath;

      private:
        std::unique_ptr< ConfigReader > mConfigReader;

        std::vector< std::string > mConfigFiles;

        // Default Filepath information
        static const std::string DEFAULT_OUTPUT_DIR_NAME;
        static const std::string DEFAULT_ASSET_DIR_NAME;
        static const std::string DEFAULT_ASSET_FONTS_DIR_NAME;
        static const std::string DEFAULT_ASSET_TEXTURES_DIR_NAME;

      private:
        unsigned int mDirectoryBits : 3;
    };

    inline void Configuration::setDirectoryInit( DirectoryIDs directoryID )
    {
        mDirectoryBits = mDirectoryBits | 1 << directoryID;

        return;
    }

    inline bool Configuration::isInitialized() const
    {
        // 7 is from 2^3 - 1
        return mDirectoryBits == ( std::pow( 2, (float)DirectoryIDs::SIZE ) ) - 1;
    }

    inline bool Configuration::isConfigInitialized() const
    {
        // 2 is the ENUM value for CONFIG
        return mDirectoryBits == std::pow( 2, (float)DirectoryIDs::CONFIG );
    }
}