#include "core/Configuration/Configuration.hpp"

#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"
#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"
#include "core/Configuration/ConfigTree/ConfigUtils.hpp"
#include "core/Configuration/Configurables/Configurable.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Time.hpp"

#include <cassert>
#include <sstream>
#include <iostream>

// These can be changed via config files
const std::string Core::Configuration::DEFAULT_OUTPUT_DIR_NAME = "output";
const std::string Core::Configuration::DEFAULT_ASSET_DIR_NAME = "assets";
const std::string Core::Configuration::DEFAULT_ASSET_FONTS_DIR_NAME = "fonts";
const std::string Core::Configuration::DEFAULT_ASSET_TEXTURES_DIR_NAME = "textures";

Core::ConfigSpec::ConfigSpec() :
    rootConfigFile( "" ),
    configDirectory( "" ),
    configReader( nullptr )
{}

Core::Configuration::Configuration( ConfigSpec configSpec ) :
    mProjectDirectory( PROJECT_DIR ),
    mRootFile( std::move( configSpec.rootConfigFile ) ),
    mConfigDirectory( std::move( configSpec.configDirectory ) ),
    mConfigDirPath( mProjectDirectory + "/" + mConfigDirectory ),
    mOutputDirPath(),
    mAssetDirPath(),
    mAssetFontsDirPath(),
    mAssetTexturesDirPath(),
    mConfigReader( std::move( configSpec.configReader ) ),
    mConfigFiles(),
    mDirectoryBits( 0 )
{
    if constexpr (Utility::CAN_LOG)
    {
        Utility::createGlobalLogger();
        initializeGlobalLogger();
    }

    // Check to see if config directory is valid
    if (!( std::filesystem::is_directory( mConfigDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Config Directory WAS NOT FOUND --> " + mConfigDirPath );

        throw ConfigurationException( std::string( "Config directory could NOT be found" + mConfigDirPath ).c_str() );
    }

    if (mRootFile.empty())
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( "Root file was NOT populated " );

        throw ConfigurationException( std::string( "Root file was NOT populated" ).c_str() );
    }

    if (!mConfigReader)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( "ConfigReader is NULL" );

        throw ConfigurationException( std::string( "ConfigReader is NULL" ).c_str() );
    }

    Configuration::setDirectoryInit( DirectoryIDs::CONFIG );

    // Populates ConfigurationTree
    // Initializes Output Directory
    // Initializes Assets Directory
    this->configure();
}

void Core::Configuration::configure()
{
    parse();
    initializeOutputDirectory();
    initializeAssetsDirectory();

    return;
}

// The following field needs to be read in by CONFIG file
// 1. OUTPUT_DIR_NAME
void Core::Configuration::initializeOutputDirectory()
{
    // Get configured output directory IF SET
    std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();
    auto configuredOutDirectory =
        ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration", "out_directory" );

    std::filesystem::path outputDirectoryPath = mProjectDirectory + "/";
    outputDirectoryPath += configuredOutDirectory.value_or( DEFAULT_OUTPUT_DIR_NAME );

    // Creates the "output" directory
    if (!( std::filesystem::is_directory( outputDirectoryPath ) ))
    {
        if (!( std::filesystem::create_directory( outputDirectoryPath ) ))
        {
            if constexpr (Utility::CAN_LOG)
                Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                    "Could NOT create output DIRECTORY --> " + outputDirectoryPath.string() );

            std::string exceptionMessage =
                "Output Directory: " + outputDirectoryPath.string() + " could NOT be created";
            throw ConfigurationException( exceptionMessage.c_str() );
        }
    }

    std::tm now_tm = Utility::getCurrentSystemTime();

    // Creates the folder name for the current iteration
    std::stringstream folderName;
    folderName << "App_";
    folderName << std::put_time( &now_tm, "%Y-%m-%d_%H-%M-%S" );

    // Add folderName to already existing output directory path
    outputDirectoryPath += "/" + folderName.str();
    mOutputDirPath = outputDirectoryPath;

    // Creates the "App_" directory with the current time
    if (!( std::filesystem::create_directory( mOutputDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Could NOT create output APP_ DIRECTORY --> " + mOutputDirPath );

        std::string exceptionMessage = "APP_Directory: " + mOutputDirPath + " could NOT be created ";
        throw ConfigurationException( exceptionMessage.c_str() );
    }

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry( mOutputDirPath );

    Configuration::setDirectoryInit( DirectoryIDs::OUTPUT );

    return;
}

void Core::Configuration::initializeAssetsDirectory()
{
    // Look up configurable values

    std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();
    auto configuredAssetDir =
        ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration", "asset_directory" );

    auto configuredAssetFontDir =
        ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration", "asset_font_directory" );

    auto configuredAssetTextureDir =
        ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration", "asset_texture_directory" );

    // Get and save the Asset file path
    mAssetDirPath = mProjectDirectory + "/";
    mAssetDirPath += configuredAssetDir.value_or( DEFAULT_ASSET_DIR_NAME );

    mAssetFontsDirPath = mAssetDirPath + "/";
    mAssetFontsDirPath += configuredAssetFontDir.value_or( DEFAULT_ASSET_FONTS_DIR_NAME );

    mAssetTexturesDirPath = mAssetDirPath + "/";
    mAssetTexturesDirPath += configuredAssetTextureDir.value_or( DEFAULT_ASSET_FONTS_DIR_NAME );

    // Check to see if directory is valid
    if (!( std::filesystem::is_directory( mAssetDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Directory WAS NOT FOUND --> " + mAssetDirPath );

        std::string exceptionMessage = "Asset Directory: " + mAssetDirPath + " could NOT be found";
        throw ConfigurationException( exceptionMessage.c_str() );
    }

    if (!( std::filesystem::is_directory( mAssetFontsDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Font Directory WAS NOT FOUND --> " + mAssetFontsDirPath );

        std::string exceptionMessage = "Asset Font Directory: " + mAssetFontsDirPath + " could NOT be found";
        throw ConfigurationException( exceptionMessage.c_str() );
    }

    if (!( std::filesystem::is_directory( mAssetTexturesDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Texture Directory WAS NOT FOUND --> " + mAssetTexturesDirPath );

        std::string exceptionMessage = "Asset Texture Directory: " + mAssetTexturesDirPath + " could NOT be found";
        throw ConfigurationException( exceptionMessage.c_str() );
    }

    Configuration::setDirectoryInit( DirectoryIDs::ASSETS );

    return;
}

// Parses the configuration files, and populates the ConfigurationTree
void Core::Configuration::parse()
{
    if (!isConfigInitialized())
    {
        std::string message = "Could NOT parse, Config DIRECTORY was NOT SET";

        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( message );

        throw ConfigurationException( message.c_str() );
    }

    if (!mConfigReader)
    {
        std::string message = "Could NOT parse, ConfigReader is NULL";

        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( message );

        throw ConfigurationException( message.c_str() );
    }

    // PARSE ROOT FILE ---- Populates ConfigurationTree
    const auto& [ rootFileParsed, rootLogStr ] = parseRootFile();
    if (!rootFileParsed)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( rootLogStr );

        throw ConfigurationException( rootLogStr.c_str() );
    }

    // PARSE CONFIG FILES ---- Populates ConfigurationTree
    const auto& [ configFilesParsed, configLogStr ] = parseConfigFiles();
    if (!configFilesParsed)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( configLogStr );

        throw ConfigurationException( configLogStr.c_str() );
    }

    return;
}

std::pair< bool, std::string > Core::Configuration::parseRootFile()
{
    // Build RootfilePath
    std::string rootFilePath = mConfigDirPath + "/" + mRootFile;

    if constexpr (Utility::CAN_LOG)
        Utility::LogRegistry::instance()->getGlobalLogger()->logDebug( "Parsing config file: " + rootFilePath );

    std::shared_ptr< ConfigNode > rootConfigNode = nullptr;
    try
    {
        auto retStatus = mConfigReader->readFile( std::filesystem::path( rootFilePath ), rootConfigNode );

        if (!retStatus.first)
            return retStatus;

        ConfigurationTree::instance()->attachConfigNode( rootConfigNode );

        std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();

        // Get files from Config Tree
        auto coreConfigFile =
            ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration_Files", "core_configurables" );

        if (coreConfigFile)
            mConfigFiles.push_back( std::string( mConfigDirPath + "/" + *coreConfigFile ) );

        auto prefabConfigFile =
            ConfigUtils::findValueByNode< std::string >( rootNode, "root.Configuration_Files", "prefabs" );

        if (prefabConfigFile)
            mConfigFiles.push_back( std::string( mConfigDirPath + "/" + *prefabConfigFile ) );
    }
    catch (const toml::parse_error& e)
    {
        return std::make_pair( false, "TOML parse error in file: " + rootFilePath + " - " + e.what() );
    }

    return std::make_pair( true, std::string( "" ) );
}

std::pair< bool, std::string > Core::Configuration::parseConfigFiles()
{
    for (const auto& configFile : mConfigFiles)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logDebug( "Parsing config file: " + configFile );

        std::shared_ptr< ConfigNode > fileConfigNode = nullptr;
        try
        {
            auto retStatus = mConfigReader->readFile( std::filesystem::path( configFile ), fileConfigNode );

            // IF we fail to read a file, then just fail fast
            if (!retStatus.first)
                return retStatus;

            ConfigurationTree::instance()->attachConfigNode( fileConfigNode );
        }
        catch (const toml::parse_error& e)
        {
            return std::make_pair( false, "TOML parse error in file: " + configFile + " - " + e.what() );
        }
    }

    return std::make_pair( true, std::string( "" ) );
}

std::pair< bool, std::filesystem::path > Core::Configuration::buildConfigFilePath( const std::string& configFile )
{
    bool fileExists = true;
    std::filesystem::path configFilePath = mConfigDirPath + "/" + configFile;

    // Check if file exists
    if (!std::filesystem::is_regular_file( configFilePath ))
        fileExists = false;

    return std::make_pair( fileExists, configFilePath );
}

void Core::Configuration::initializeGlobalLogger()
{
    // Get Global Logger
    std::shared_ptr< Utility::Logger > cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    if (cLogger)
    {
        auto globalConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
        cLogger->addSink( globalConsoleSink );

        std::string logMessage;
        logMessage += "Initialized Global Logger";
        cLogger->logInfo( logMessage );
    }

    return;
}