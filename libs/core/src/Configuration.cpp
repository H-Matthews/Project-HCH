#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <string>
#include <filesystem>

const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";
const std::string Core::Configuration::CONFIG_DIR_NAME = "configs";
const std::string Core::Configuration::MAIN_FILE_NAME = "base.ini";

Core::Configuration::Configuration() :
    mConfigDirPath(),
    mOutputDirPath(),
    mConfigParserMap(),
    mProjectDirectory(PROJECT_DIR)
{
    if constexpr (Utility::CAN_LOG)
    {
        Utility::createGlobalLogger();
        initializeGlobalLogger();
    }

    initializeParsers();
}

void Core::Configuration::initializeParsers()
{
    // Populate ParserTypes to Parser extension map
    mParserTypeToExtensionMap.insert(std::make_pair(Core::ParserType::INI, "ini"));
    mParserTypeToExtensionMap.insert(std::make_pair(Core::ParserType::JSON, "json"));

    // Create Parsers here
    std::string parserNameID("IniParser");
    std::string parserExt("ini");
    mConfigParserMap.insert(std::make_pair(parserNameID, std::make_unique<Core::IniParser>(parserNameID, parserExt) ));

    return;
}

void Core::Configuration::parseConfigs()
{

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
    if( !(std::filesystem::is_directory(mConfigDirPath)) )
    {
        if constexpr (Utility::CAN_LOG)
           Utility::LogRegistry::instance()->getGlobalLogger()->logError("Config Directory WAS NOT FOUND --> " + mConfigDirPath );

        throw std::filesystem::filesystem_error("Config directory: " + mConfigDirPath + " could not be found", std::error_code());
    }

    // See if base file exists
    if( !(std::filesystem::is_regular_file(mConfigDirPath + "/" + MAIN_FILE_NAME)))
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError("Root Config File WAS NOT FOUND --> " 
                + mConfigDirPath + "/" + MAIN_FILE_NAME);

        throw std::filesystem::filesystem_error("Main config file: " + MAIN_FILE_NAME + " could not be found", std::error_code());
    }

    initializeConfigFiles();
}

// The following field needs to be read in by CONFIG file
// 1. OUTPUT_DIR_NAME
void Core::Configuration::initializeOutputDirectory()
{
    std::string outputDirectoryPath;
    outputDirectoryPath += mProjectDirectory + "/" + OUTPUT_DIR_NAME;

    // Creates the "output" directory 
    if( !(std::filesystem::is_directory(outputDirectoryPath)) )
    {
        if( !(std::filesystem::create_directory(outputDirectoryPath)) )
        {
            if constexpr (Utility::CAN_LOG)
                Utility::LogRegistry::instance()->getGlobalLogger()->logError("Could NOT create output DIRECTORY --> " + outputDirectoryPath);

            throw std::filesystem::filesystem_error("Output Directory: " + outputDirectoryPath + " could NOT be created", std::error_code());
        }
    }

    // Get Time in a broken down structure
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&nowTime);

    // Creates the folder name for the current iteration
    std::stringstream folderName;
    folderName << "App_";
    folderName << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S");

    // Add folderName to already existing output directory path
    outputDirectoryPath += "/" + folderName.str();
    mOutputDirPath = outputDirectoryPath;

    // Creates the "App_" directory with the current time
    if( !(std::filesystem::create_directory(mOutputDirPath)) )
    {
        if constexpr (Utility::CAN_LOG)
            Utility::LogRegistry::instance()->getGlobalLogger()->logError("Could NOT create output APP_ DIRECTORY --> " + mOutputDirPath);

        throw std::filesystem::filesystem_error("APP_ Directory: " + mOutputDirPath + " could NOT be created", std::error_code());
    }

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry(mOutputDirPath);
}

void Core::Configuration::initializeConfigFiles()
{

    return;
}

void Core::Configuration::initializeGlobalLogger()
{
    // Get Global Logger
    std::shared_ptr< Utility::Logger > cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    if(cLogger)
    {
        auto globalConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
        cLogger->addSink(globalConsoleSink);

        std::string logMessage;
        logMessage += "Initialized Global Logger: " + mOutputDirPath;
        cLogger->logInfo(logMessage);
    }

    return;
}

const std::string Core::Configuration::getOutDirPath()
{
    return mOutputDirPath;
}