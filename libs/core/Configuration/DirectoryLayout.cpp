#include "core/Configuration/DirectoryLayout.hpp"

#include "core/Exceptions/ConfigurationException.hpp"
#include "utility/Time.hpp"

#include <filesystem>
#include <iomanip>
#include <optional>
#include <sstream>

Core::DirectoryLayout::DirectoryLayout( const ConfigSection* config ) :
    mProjectDirectory( PROJECT_DIR )
{
    initializeOutputDirectory( config );
    initializeAssetsDirectory( config );
}

void Core::DirectoryLayout::initializeOutputDirectory( const ConfigSection* config )
{
    std::optional< std::string > configuredOutDirectory;
    if (config)
        configuredOutDirectory = config->getString( SectionKeys::OUT_DIR );

    std::filesystem::path outputDirectoryPath = mProjectDirectory + "/";
    outputDirectoryPath += configuredOutDirectory.value_or( std::string( DEFAULT_OUTPUT_DIR_NAME ) );

    if (!( std::filesystem::is_directory( outputDirectoryPath ) ))
    {
        if (!( std::filesystem::create_directory( outputDirectoryPath ) ))
        {
            throw ConfigurationException(
                ( "Output Directory: " + outputDirectoryPath.string() + " could NOT be created" ).c_str() );
        }
    }

    std::tm now_tm = Utility::getCurrentSystemTime();

    std::stringstream folderName;
    folderName << "App_";
    folderName << std::put_time( &now_tm, "%Y-%m-%d_%H-%M-%S" );

    outputDirectoryPath += "/" + folderName.str();
    mOutputDirPath = outputDirectoryPath.string();

    if (!( std::filesystem::create_directory( mOutputDirPath ) ))
    {
        throw ConfigurationException( ( "APP_ Directory: " + mOutputDirPath + " could NOT be created" ).c_str() );
    }
}

void Core::DirectoryLayout::initializeAssetsDirectory( const ConfigSection* config )
{
    std::optional< std::string > configuredAssetDir;
    std::optional< std::string > configuredAssetFontDir;
    std::optional< std::string > configuredAssetTextureDir;

    if (config)
    {
        configuredAssetDir = config->getString( SectionKeys::ASSET_DIR );
        configuredAssetFontDir = config->getString( SectionKeys::FONT_DIR );
        configuredAssetTextureDir = config->getString( SectionKeys::TEXTURE_DIR );
    }

    mAssetDirPath = mProjectDirectory + "/";
    mAssetDirPath += configuredAssetDir.value_or( std::string( DEFAULT_ASSET_DIR_NAME ) );

    mAssetFontsDirPath = mAssetDirPath + "/";
    mAssetFontsDirPath += configuredAssetFontDir.value_or( std::string( DEFAULT_ASSET_FONTS_DIR_NAME ) );

    mAssetTexturesDirPath = mAssetDirPath + "/";
    mAssetTexturesDirPath += configuredAssetTextureDir.value_or( std::string( DEFAULT_ASSET_TEXTURES_DIR_NAME ) );

    if (!( std::filesystem::is_directory( mAssetDirPath ) ))
        throw ConfigurationException( ( "Asset Directory: " + mAssetDirPath + " could NOT be found" ).c_str() );

    if (!( std::filesystem::is_directory( mAssetFontsDirPath ) ))
    {
        throw ConfigurationException(
            ( "Asset Font Directory: " + mAssetFontsDirPath + " could NOT be found" ).c_str() );
    }

    if (!( std::filesystem::is_directory( mAssetTexturesDirPath ) ))
    {
        throw ConfigurationException(
            ( "Asset Texture Directory: " + mAssetTexturesDirPath + " could NOT be found" ).c_str() );
    }
}
