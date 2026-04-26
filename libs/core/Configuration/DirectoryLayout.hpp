#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <string>

namespace Core
{
    /**
     * Resolves and creates the on-disk directories the application depends on
     * (output, assets, fonts, textures) from a ConfigSection. Constructed once
     * by Application after Configuration has parsed the config files.
     * Construction either fully succeeds or throws ConfigurationException.
     */
    class DirectoryLayout
    {
      public:
        explicit DirectoryLayout( const ConfigSection* config );

        const std::string& projectDirectory() const { return mProjectDirectory; }
        const std::string& outputDirectory() const { return mOutputDirPath; }
        const std::string& assetDirectory() const { return mAssetDirPath; }
        const std::string& assetFontsDirectory() const { return mAssetFontsDirPath; }
        const std::string& assetTexturesDirectory() const { return mAssetTexturesDirPath; }

      private:
        void initializeOutputDirectory( const ConfigSection* config );
        void initializeAssetsDirectory( const ConfigSection* config );

        std::string mProjectDirectory;

        std::string mOutputDirPath;
        std::string mAssetDirPath;
        std::string mAssetFontsDirPath;
        std::string mAssetTexturesDirPath;

        static constexpr std::string_view DEFAULT_OUTPUT_DIR_NAME = "output";
        static constexpr std::string_view DEFAULT_ASSET_DIR_NAME = "assets";
        static constexpr std::string_view DEFAULT_ASSET_FONTS_DIR_NAME = "fonts";
        static constexpr std::string_view DEFAULT_ASSET_TEXTURES_DIR_NAME = "textures";

        struct SectionKeys
        {
            static constexpr std::string_view OUT_DIR = "out_directory";
            static constexpr std::string_view ASSET_DIR = "asset_directory";
            static constexpr std::string_view FONT_DIR = "asset_font_directory";
            static constexpr std::string_view TEXTURE_DIR = "asset_texture_directory";
        };
    };
}
