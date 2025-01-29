#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <sstream>
#include <filesystem>

const std::string Core::Configuration::CONFIG_DIR_NAME = "configs";
const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";

Core::Configuration::Configuration() :
    mConfigDirPath(),
    mOutputDirPath(),
    mProjectDirectory(PROJECT_DIR)
{
    if constexpr (Utility::CAN_LOG)
        Utility::createGlobalLogger();
}

bool Core::Configuration::initializeIteration()
{
    bool initialized = true;

    if constexpr (Utility::CAN_LOG)
        initializeGlobalLogger();

    initialized = initializeOutputDirectory();
    initialized = initializeConfigDirectory();

    if(Utility::CAN_LOG && !initialized)
        Utility::LogRegistry::instance()->getGlobalLogger()->logError("Application FAILED Configuration Initialization");

    return initialized;
}

bool Core::Configuration::initializeOutputDirectory()
{
    bool initOutputDir = true;

    std::stringstream outputDirectoryPath;
    outputDirectoryPath << mProjectDirectory << "/" << OUTPUT_DIR_NAME;

    // If directory DOES NOT exist, create it
    if( !(std::filesystem::is_directory(outputDirectoryPath.str())) )
    {   
        if( !(std::filesystem::create_directory(outputDirectoryPath.str())) )
        {
            initOutputDir = false;
        }
    }

    // Get Time in a broken down structure
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&nowTime);

    // Create folderName
    std::stringstream folderName;
    folderName << "App_";
    folderName << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S");

    // Add folderName to already existing output directory path
    outputDirectoryPath << "/" << folderName.str();
    mOutputDirPath = outputDirectoryPath.str();

    // Create directory
    if( !(std::filesystem::create_directory(outputDirectoryPath.str())) )
        initOutputDir = false;

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry(mOutputDirPath);

    return initOutputDir;
}

bool Core::Configuration::initializeConfigDirectory()
{
    bool initConfigDir = true;

    std::stringstream configDirectoryPath;

    configDirectoryPath << mProjectDirectory << "/" << CONFIG_DIR_NAME;
    mConfigDirPath = configDirectoryPath.str();

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
}

void Core::Configuration::loadSettings()
{
    // TODO: Define how we will read data into the program (ini?, JSON?)
}

const std::string Core::Configuration::getOutDirPath()
{
    return mOutputDirPath;
}