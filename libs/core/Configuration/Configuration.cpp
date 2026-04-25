#include "core/Configuration/Configuration.hpp"

#include "utility/Logging/LogRegistry.hpp"
#include "utility/Time.hpp"

#include <sstream>
#include <iostream>

const std::string Core::Configuration::DEFAULT_OUTPUT_DIR_NAME = "output";
const std::string Core::Configuration::DEFAULT_ASSET_DIR_NAME = "assets";
const std::string Core::Configuration::DEFAULT_ASSET_FONTS_DIR_NAME = "fonts";
const std::string Core::Configuration::DEFAULT_ASSET_TEXTURES_DIR_NAME = "textures";

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
    mRootSection( nullptr ),
    mParsedSections(),
    mConfigFiles(),
    mDirectoryBits( 0 )
{
    if constexpr (Utility::CAN_LOG)
    {
        Utility::createGlobalLogger();
        initializeGlobalLogger();
    }

    if (!( std::filesystem::is_directory( mConfigDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Config Directory WAS NOT FOUND --> " + mConfigDirPath );

        throw ConfigurationException( ( "Config directory could NOT be found: " + mConfigDirPath ).c_str() );
    }

    if (mRootFile.empty())
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( "Root file was NOT populated" );

        throw ConfigurationException( "Root file was NOT populated" );
    }

    if (!mConfigReader)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( "ConfigReader is NULL" );

        throw ConfigurationException( "ConfigReader is NULL" );
    }

    Configuration::setDirectoryInit( DirectoryIDs::CONFIG );

    this->configure();
}

void Core::Configuration::configure()
{
    parse();
    initializeOutputDirectory();
    initializeAssetsDirectory();
}

void Core::Configuration::initializeOutputDirectory()
{
    std::optional< std::string > configuredOutDirectory;
    if (mRootSection)
    {
        auto configSection = mRootSection->getSection( RootConfigSection::NAME );
        if (configSection)
            configuredOutDirectory = configSection->getString( RootConfigSection::OUT_DIR );
    }

    std::filesystem::path outputDirectoryPath = mProjectDirectory + "/";
    outputDirectoryPath += configuredOutDirectory.value_or( DEFAULT_OUTPUT_DIR_NAME );

    if (!( std::filesystem::is_directory( outputDirectoryPath ) ))
    {
        if (!( std::filesystem::create_directory( outputDirectoryPath ) ))
        {
            if constexpr (Utility::CAN_LOG)
                Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                    "Could NOT create output DIRECTORY --> " + outputDirectoryPath.string() );

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
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Could NOT create output APP_ DIRECTORY --> " + mOutputDirPath );

        throw ConfigurationException( ( "APP_ Directory: " + mOutputDirPath + " could NOT be created" ).c_str() );
    }

    Utility::LogRegistry::instance()->configureRegistry( mOutputDirPath );

    Configuration::setDirectoryInit( DirectoryIDs::OUTPUT );
}

void Core::Configuration::initializeAssetsDirectory()
{
    std::optional< std::string > configuredAssetDir;
    std::optional< std::string > configuredAssetFontDir;
    std::optional< std::string > configuredAssetTextureDir;

    if (mRootSection)
    {
        auto configSection = mRootSection->getSection( RootConfigSection::NAME );
        if (configSection)
        {
            configuredAssetDir = configSection->getString( RootConfigSection::ASSET_DIR );
            configuredAssetFontDir = configSection->getString( RootConfigSection::FONT_DIR );
            configuredAssetTextureDir = configSection->getString( RootConfigSection::TEXTURE_DIR );
        }
    }

    mAssetDirPath = mProjectDirectory + "/";
    mAssetDirPath += configuredAssetDir.value_or( DEFAULT_ASSET_DIR_NAME );

    mAssetFontsDirPath = mAssetDirPath + "/";
    mAssetFontsDirPath += configuredAssetFontDir.value_or( DEFAULT_ASSET_FONTS_DIR_NAME );

    mAssetTexturesDirPath = mAssetDirPath + "/";
    mAssetTexturesDirPath += configuredAssetTextureDir.value_or( DEFAULT_ASSET_TEXTURES_DIR_NAME );

    if (!( std::filesystem::is_directory( mAssetDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Directory WAS NOT FOUND --> " + mAssetDirPath );

        throw ConfigurationException( ( "Asset Directory: " + mAssetDirPath + " could NOT be found" ).c_str() );
    }

    if (!( std::filesystem::is_directory( mAssetFontsDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Font Directory WAS NOT FOUND --> " + mAssetFontsDirPath );

        throw ConfigurationException(
            ( "Asset Font Directory: " + mAssetFontsDirPath + " could NOT be found" ).c_str() );
    }

    if (!( std::filesystem::is_directory( mAssetTexturesDirPath ) ))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Asset Texture Directory WAS NOT FOUND --> " + mAssetTexturesDirPath );

        throw ConfigurationException(
            ( "Asset Texture Directory: " + mAssetTexturesDirPath + " could NOT be found" ).c_str() );
    }

    Configuration::setDirectoryInit( DirectoryIDs::ASSETS );
}

void Core::Configuration::parse()
{
    if (!isConfigInitialized())
    {
        const std::string message = "Could NOT parse, Config DIRECTORY was NOT SET";

        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( message );

        throw ConfigurationException( message.c_str() );
    }

    const auto& [ rootFileParsed, rootLogStr ] = parseRootFile();
    if (!rootFileParsed)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( rootLogStr );

        throw ConfigurationException( rootLogStr.c_str() );
    }

    const auto& [ configFilesParsed, configLogStr ] = parseConfigFiles();
    if (!configFilesParsed)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError( configLogStr );

        throw ConfigurationException( configLogStr.c_str() );
    }
}

std::pair< bool, std::string > Core::Configuration::parseRootFile()
{
    const std::string rootFilePath = mConfigDirPath + "/" + mRootFile;

    if constexpr (Utility::CAN_LOG)
        Utility::LogRegistry::instance()->getGlobalLogger()->logDebug( "Parsing config file: " + rootFilePath );

    mRootSection = mConfigReader->readFile( std::filesystem::path( rootFilePath ) );
    if (!mRootSection)
        return { false, "Could not find root config file: " + rootFilePath };

    auto filesSection = mRootSection->getSection( RootFilesSection::NAME );
    if (filesSection)
    {
        if (auto coreConfigFile = filesSection->getString( RootFilesSection::CORE_CONFIGURABLES ))
            mConfigFiles.push_back( mConfigDirPath + "/" + *coreConfigFile );

        if (auto prefabConfigFile = filesSection->getString( RootFilesSection::PREFABS ))
            mConfigFiles.push_back( mConfigDirPath + "/" + *prefabConfigFile );
    }

    return { true, "" };
}

std::pair< bool, std::string > Core::Configuration::parseConfigFiles()
{
    for (const auto& configFile : mConfigFiles)
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logDebug( "Parsing config file: " + configFile );

        auto section = mConfigReader->readFile( std::filesystem::path( configFile ) );
        if (!section)
            return { false, "Could not find config file: " + configFile };

        mParsedSections.push_back( std::move( section ) );
    }

    return { true, "" };
}

std::unique_ptr< Core::ConfigSection > Core::Configuration::getSection( std::string_view name ) const
{
    for (const auto& section : mParsedSections)
    {
        auto sub = section->getSection( name );
        if (sub)
            return sub;
    }
    return nullptr;
}

void Core::Configuration::initializeGlobalLogger() const
{
    std::shared_ptr< Utility::Logger > cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    if (cLogger)
    {
        auto globalConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
        cLogger->addSink( globalConsoleSink );
        cLogger->logInfo( "Initialized Global Logger" );
    }
}
