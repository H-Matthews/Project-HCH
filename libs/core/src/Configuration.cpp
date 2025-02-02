#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <string>
#include <filesystem>

const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";

Core::Configuration::Configuration(const std::string& configDirectory) :
    mConfigDirPath(),
    mConfigDirName(configDirectory),
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

bool Core::Configuration::initializeIteration()
{
    bool initialized = true;

    initialized = initializeConfigDirectory();
    if(!initialized) { return initialized; }

    initialized = initializeOutputDirectory();

    return initialized;
}

void Core::Configuration::initializeParsers()
{
    // Create Parsers here
    std::string parserNameID("IniParser");
    std::string parserExt(".ini");
    mConfigParserMap.insert(std::make_pair(parserNameID, std::make_unique<Core::IniParser>(parserNameID, parserExt) ));

    return;
}

bool Core::Configuration::initializeConfigDirectory()
{
    bool initConfigDir = true;

    std::string configDirectoryPath;

    configDirectoryPath += mProjectDirectory + "/" + mConfigDirName;
    mConfigDirPath = configDirectoryPath;

    // Check to see if directory is valid
    if( !(std::filesystem::is_directory(mConfigDirPath)) )
    {
        initConfigDir = false;

        if constexpr (Utility::CAN_LOG)
        {
            // Log to Global Logger
            Utility::LogRegistry::instance()->getGlobalLogger()->logError("Config Directory WAS NOT FOUND --> " + mConfigDirPath );
        }

    }

    return initConfigDir;
}

// The following field needs to be read in by CONFIG file
// 1. OUTPUT_DIR_NAME
bool Core::Configuration::initializeOutputDirectory()
{
    bool initOutputDir = true;

    std::string outputDirectoryPath;
    outputDirectoryPath += mProjectDirectory + "/" + OUTPUT_DIR_NAME;

    // Creates the "output" directory 
    if( !(std::filesystem::is_directory(outputDirectoryPath)) )
    {
        if( !(std::filesystem::create_directory(outputDirectoryPath)) )
        {
            initOutputDir = false;

            if constexpr (Utility::CAN_LOG)
            {
                // Log to Global Logger
                Utility::LogRegistry::instance()->getGlobalLogger()->logError("Could NOT create output DIRECTORY --> " + outputDirectoryPath);
            }

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
        initOutputDir = false;

        if constexpr (Utility::CAN_LOG)
        {
            // Log to Global Logger
            Utility::LogRegistry::instance()->getGlobalLogger()->logError("Could NOT create output APP_ DIRECTORY --> " + mOutputDirPath);
        }

    }

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry(mOutputDirPath);

    return initOutputDir;
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