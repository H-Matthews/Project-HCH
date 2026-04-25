#pragma once

#include "core/Exceptions/ConfigurationException.hpp"
#include "core/Configuration/ConfigReader/ConfigReader.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include <string>
#include <cmath>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <filesystem>

namespace Core
{
    struct ConfigSpec
    {
        std::string rootConfigFile;
        std::string configDirectory;

        std::unique_ptr<ConfigReader> configReader;

        ConfigSpec() = default;
        ConfigSpec(ConfigSpec&&) = default;
        ConfigSpec& operator=(ConfigSpec&&) = default;
    };

    enum DirectoryIDs
    {
        OUTPUT = 0,
        CONFIG,
        ASSETS,
        SIZE
    };

    class Configuration
    {
      public:
        explicit Configuration(ConfigSpec configSpec);

        std::unique_ptr<ConfigSection> getSection(std::string_view name) const;

        void setDirectoryInit(DirectoryIDs directoryID);

        bool isInitialized() const;
        bool isConfigInitialized() const;

        ~Configuration() = default;

      private:
        void configure();

        void initializeOutputDirectory();
        void initializeAssetsDirectory();

        void parse();
        std::pair<bool, std::string> parseRootFile();
        std::pair<bool, std::string> parseConfigFiles();

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
        std::unique_ptr<ConfigReader> mConfigReader;

        std::unique_ptr<ConfigSection> mRootSection;
        std::vector<std::unique_ptr<ConfigSection>> mParsedSections;

        std::vector<std::string> mConfigFiles;

        static const std::string DEFAULT_OUTPUT_DIR_NAME;
        static const std::string DEFAULT_ASSET_DIR_NAME;
        static const std::string DEFAULT_ASSET_FONTS_DIR_NAME;
        static const std::string DEFAULT_ASSET_TEXTURES_DIR_NAME;

      private:
        unsigned int mDirectoryBits : 3;
    };

    inline void Configuration::setDirectoryInit(DirectoryIDs directoryID)
    {
        mDirectoryBits = mDirectoryBits | 1 << directoryID;
    }

    inline bool Configuration::isInitialized() const
    {
        return mDirectoryBits == (std::pow(2, (float)DirectoryIDs::SIZE)) - 1;
    }

    inline bool Configuration::isConfigInitialized() const
    {
        return mDirectoryBits == std::pow(2, (float)DirectoryIDs::CONFIG);
    }
}
