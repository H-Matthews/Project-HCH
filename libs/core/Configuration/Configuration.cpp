#include "core/Configuration/Configuration.hpp"

#include "core/Configuration/ConfigFileID.hpp"

#include "utility/Logging/LogRegistry.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

const std::string Core::Configuration::ROOT_CONFIG_FILE_NAME = "root.toml";
const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";
const std::string Core::Configuration::CONFIG_DIR_NAME = "configs";
const std::string Core::Configuration::ASSET_DIR_NAME = "assets";
const std::string Core::Configuration::ASSET_FONTS_DIR_NAME = "fonts";
const std::string Core::Configuration::ASSET_TEXTURES_DIR_NAME = "textures";

Core::Configuration::Configuration() :
    ConfigurationI( PROJECT_DIR ),
    mConfigFileIDs(),
    mConfigFiles()
{
    if constexpr ( Utility::CAN_LOG )
    {
        Utility::createGlobalLogger();
        initializeGlobalLogger();
    }

    if ( !initializeConfigFileIDs() )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( "Missing ConfigFileID" );
    }
}

bool Core::Configuration::initializeConfigFileIDs()
{
    mConfigFileIDs.push_back( ConfigFileID::CORE_CONFIGURABLES );
    mConfigFileIDs.push_back( ConfigFileID::PREFABS );

    return mConfigFileIDs.size() == (std::size_t)ConfigFileID::SIZE;
}

// The following field needs to be read in by CONFIG file
// 1. OUTPUT_DIR_NAME
void Core::Configuration::initializeOutputDirectory()
{
    std::string outputDirectoryPath;
    outputDirectoryPath += mProjectDirectory + "/" + OUTPUT_DIR_NAME;

    // Creates the "output" directory
    if ( !( std::filesystem::is_directory( outputDirectoryPath ) ) )
    {
        if ( !( std::filesystem::create_directory( outputDirectoryPath ) ) )
        {
            if constexpr ( Utility::CAN_LOG )
                Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                    "Could NOT create output DIRECTORY --> " + outputDirectoryPath );

            throw std::filesystem::filesystem_error(
                "Output Directory: " + outputDirectoryPath + " could NOT be created", std::error_code() );
        }
    }

    // Get Time in a broken down structure
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t( now );
    std::tm now_tm = *std::localtime( &nowTime );

    // Creates the folder name for the current iteration
    std::stringstream folderName;
    folderName << "App_";
    folderName << std::put_time( &now_tm, "%Y-%m-%d_%H-%M-%S" );

    // Add folderName to already existing output directory path
    outputDirectoryPath += "/" + folderName.str();
    mOutputDirPath = outputDirectoryPath;

    // Creates the "App_" directory with the current time
    if ( !( std::filesystem::create_directory( mOutputDirPath ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Could NOT create output APP_ DIRECTORY --> " + mOutputDirPath );

        throw std::filesystem::filesystem_error(
            "APP_ Directory: " + mOutputDirPath + " could NOT be created", std::error_code() );
    }

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry( mOutputDirPath );

    ConfigurationI::setDirectoryInit( DirectoryIDs::OUTPUT );

    return;
}

void Core::Configuration::initializeConfigDirectory()
{
    std::string configDirectoryPath;

    configDirectoryPath += mProjectDirectory + "/" + CONFIG_DIR_NAME;
    mConfigDirPath = configDirectoryPath;

    // Check to see if directory is valid
    if ( !( std::filesystem::is_directory( mConfigDirPath ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Config Directory WAS NOT FOUND --> " + mConfigDirPath );

        throw std::filesystem::filesystem_error(
            "Config directory: " + mConfigDirPath + " could not be found", std::error_code() );
    }

    ConfigurationI::setDirectoryInit( DirectoryIDs::CONFIG );

    return;
}

void Core::Configuration::initializeAssetsDirectory()
{
    // Get and save the Asset file path
    std::string assetDirectoryPath;

    assetDirectoryPath += mProjectDirectory + "/" + ASSET_DIR_NAME;
    mAssetDirPath = assetDirectoryPath;

    // Set convenience path for fonts / textures as well
    mAssetFontsDirPath = mAssetDirPath + "/" + ASSET_FONTS_DIR_NAME;
    mAssetTexturesDirPath = mAssetDirPath + "/" + ASSET_TEXTURES_DIR_NAME;

    // Check to see if directory is valid
    if ( !( std::filesystem::is_directory( mAssetDirPath ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Directory WAS NOT FOUND --> " + mAssetDirPath );

        throw std::filesystem::filesystem_error(
            "Asset directory: " + mAssetDirPath + " could not be found", std::error_code() );
    }

    if ( !( std::filesystem::is_directory( mAssetFontsDirPath ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Font Directory WAS NOT FOUND --> " + mAssetFontsDirPath );

        throw std::filesystem::filesystem_error(
            "Asset Font directory: " + mAssetFontsDirPath + " could not be found", std::error_code() );
    }

    if ( !( std::filesystem::is_directory( mAssetTexturesDirPath ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Texture Directory WAS NOT FOUND --> " + mAssetTexturesDirPath );

        throw std::filesystem::filesystem_error(
            "Asset Texture directory: " + mAssetTexturesDirPath + " could not be found", std::error_code() );
    }

    ConfigurationI::setDirectoryInit( DirectoryIDs::ASSETS );

    return;
}

bool Core::Configuration::parse()
{
    if ( !ConfigurationI::isInitialized() )
    {
        // TODO: Write a function to get the exact directories that still need to be intialized
        if constexpr ( Utility::CAN_LOG )
        {
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Could NOT CONFIGURE, directories are NOT initialized --> " );
        }

        return false;
    }

    // PARSE ROOT FILE --- Populates mConfigFiles
    auto retRootPair = parseRootFile();
    if ( !retRootPair.first )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( retRootPair.second );

        return false;
    }

    // PARSE CONFIG FILES ---- Populates ConfigRegistry (Doesnt exist yet)
    auto retConfigPair = parseConfigFiles();
    if ( !retConfigPair.first )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( retConfigPair.second );

        return false;
    }

    return true;
}

/**
 * Parses the root.toml file
 */
std::pair< bool, std::string > Core::Configuration::parseRootFile()
{
    // Parse Root.toml for Configuration File ID mappings
    std::string rootFilePath = mConfigDirPath + "/" + ROOT_CONFIG_FILE_NAME;
    if ( ( !std::filesystem::is_regular_file( rootFilePath ) ) )
    {
        const std::string errString = "Could NOT FIND root file ---->" + rootFilePath;
        return std::make_pair( false, errString );
    }

    toml::table tbl;
    try
    {
        tbl = toml::parse_file( rootFilePath );
    }
    catch ( const toml::parse_error& err )
    {
        return std::make_pair( false, err.what() );
    }

    // PARSE CONFIGURATION FILE PATHS
    const std::string configFileKey( "Configuration_Files" );

    for ( const auto& configID : mConfigFileIDs )
    {
        std::string configIDString = enumToString( configID );

        std::optional< std::string > str1 = tbl[ configFileKey ][ configIDString ].value< std::string >();

        if ( str1.has_value() )
        {
            // Build filepath
            std::filesystem::path filePath = mConfigDirPath + "/" + str1.value();
            if ( ( !std::filesystem::exists( filePath ) ) )
            {
                if constexpr ( Utility::CAN_LOG )
                {
                    Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                        std::string( "Could NOT find " + filePath.string() ) );
                }
                continue;
            }

            mConfigFiles[ configID ] = std::make_pair( configIDString, filePath );
        }
    }

    return std::make_pair( true, std::string( "" ) );
}

std::pair< bool, std::string > Core::Configuration::parseConfigFiles()
{
    for ( const auto& configFileID : mConfigFileIDs )
    {
        auto it = mConfigFiles.find( configFileID );
        if ( it != mConfigFiles.end() )
        {
            toml::table tomlTable;
            try
            {
                tomlTable = toml::parse_file( it->second.second.string() );

                handleConfigFile( tomlTable, it->first );
            }
            catch ( const toml::parse_error& err )
            {
                return std::make_pair( false, err.what() );
            }
        }
    }

    return std::make_pair( true, std::string( "" ) );
}

std::pair< bool, std::string > Core::Configuration::handleConfigFile(
    const toml::table& tomlTable, ConfigFileID configFileID )
{

    return std::make_pair( true, "" );
}

void Core::Configuration::initializeGlobalLogger()
{
    // Get Global Logger
    std::shared_ptr< Utility::Logger > cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    if ( cLogger )
    {
        auto globalConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
        cLogger->addSink( globalConsoleSink );

        std::string logMessage;
        logMessage += "Initialized Global Logger";
        cLogger->logInfo( logMessage );
    }

    return;
}