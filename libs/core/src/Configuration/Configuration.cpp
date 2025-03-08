#include "core/inc/Configuration/Configuration.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <cassert>
#include <sstream>

const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";
const std::string Core::Configuration::CONFIG_DIR_NAME = "configs";
const std::string Core::Configuration::MAIN_FILE_NAME = "base.ini";

Core::Configuration::Configuration() :
    mConfigDirPath(),
    mOutputDirPath(),
    mConfigFiles(),
    mFileExtensionToIDMap(),
    mParserRegistry(),
    mParsers(),
    mProjectDirectory( PROJECT_DIR )
{
    if constexpr ( Utility::CAN_LOG )
    {
        Utility::createGlobalLogger();
        initializeGlobalLogger();
    }

    initializeParsers();
}

void Core::Configuration::initializeParsers()
{
    // Register Parsers
    registerParser< Core::IniParser >( Parsers::ID::INI );

    return;
}

std::unique_ptr< Core::Parser > Core::Configuration::createParser( Parsers::ID parserID )
{
    auto found = mParserRegistry.find( parserID );
    assert( found != mParserRegistry.end() );

    return found->second();
}

void Core::Configuration::parseConfigs()
{
    std::string filePath;
    std::ifstream fileStream;
    for ( const auto& file : mConfigFiles )
    {
        filePath = "";
        filePath = mConfigDirPath + "/" + file.mFileName + file.mFileExtension;

        // Open File
        fileStream.open( filePath, std::ifstream::in );
        if ( fileStream.is_open() )
        {
            if constexpr ( Utility::CAN_LOG )
                Utility::LogRegistry::instance()->getGlobalLogger()->logDebug( "Parsing File: " + filePath );

            // Parse File
            auto fileExtensionIT = mFileExtensionToIDMap.find( file.mFileExtension );
            mParsers[ fileExtensionIT->second ]->parseFile( fileStream );
        }
        else
        {
            if constexpr ( Utility::CAN_LOG )
                Utility::LogRegistry::instance()->getGlobalLogger()->logWarn(
                    "Could NOT Parse File: " + filePath + " File would not open" );
        }

        fileStream.close();
    }

    return;
}

void Core::Configuration::initializeIteration()
{
    initializeConfigDirectory();

    initializeOutputDirectory();
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

    // See if base file exists
    if ( !( std::filesystem::is_regular_file( mConfigDirPath + "/" + MAIN_FILE_NAME ) ) )
    {
        if constexpr ( Utility::CAN_LOG )
            Utility::LogRegistry::instance()->getGlobalLogger()->logError(
                "Root Config File WAS NOT FOUND --> " + mConfigDirPath + "/" + MAIN_FILE_NAME );

        throw std::filesystem::filesystem_error(
            "Main config file: " + MAIN_FILE_NAME + " could not be found", std::error_code() );
    }

    initializeConfigFiles();
}

void Core::Configuration::initializeConfigFiles()
{
    for ( const auto& fileEntry : std::filesystem::directory_iterator( mConfigDirPath ) )
    {
        std::filesystem::path filePath( fileEntry.path() );

        // Ensure file extension is in MAP
        const std::string fileExtensionStr = filePath.extension().string();
        if ( mFileExtensionToIDMap.find( fileExtensionStr ) == mFileExtensionToIDMap.end() )
        {
            if constexpr ( Utility::CAN_LOG )
                Utility::LogRegistry::instance()->getGlobalLogger()->logWarn(
                    "Unknown File Extension: " + fileExtensionStr + " File: " + filePath.filename().string() +
                    " will NOT be parsed" );

            if ( filePath.filename().string() == MAIN_FILE_NAME )
                throw std::filesystem::filesystem_error(
                    "Unknown Base file extension: " + fileExtensionStr, std::error_code() );

            continue;
        }

        // IF the file extension exists, then that implies that we registered a parser to that extension
        // Add to vector
        FileInformation fileInfo( filePath.stem().string(), filePath.extension().string() );
        mConfigFiles.push_back( fileInfo );
    }

    // Create Parser Objects
    for ( const auto& file : mConfigFiles )
    {
        // Ensure File Extension is registered to a parser
        // At this point it should be
        auto fileExtensionIT = mFileExtensionToIDMap.find( file.mFileExtension );
        if ( fileExtensionIT == mFileExtensionToIDMap.end() )
            continue;

        // Ensure Parser has NOT been created already
        auto parserIT = mParsers.find( fileExtensionIT->second );
        if ( parserIT != mParsers.end() )
            continue;

        // Create Parser
        mParsers[ fileExtensionIT->second ] = createParser( fileExtensionIT->second );
    }
    return;
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
        logMessage += "Initialized Global Logger: " + mOutputDirPath;
        cLogger->logInfo( logMessage );
    }

    return;
}

const std::string Core::Configuration::getOutDirPath()
{
    return mOutputDirPath;
}