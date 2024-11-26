#include "core/inc/Configuration.hpp"

#include "utility/inc/Logging/LogRegistry.hpp"

#include <sstream>
#include <filesystem>
#include <iostream>


const std::string Core::Configuration::CONFIG_DIR_NAME = "configs";
const std::string Core::Configuration::OUTPUT_DIR_NAME = "output";

Core::Configuration::Configuration() :
    mConfigDirPath(),
    mOutputDirPath()
{
    // Establish the GlobalLogger and store in LogRegistry
    Utility::createGlobalLogger();
}

void Core::Configuration::initializeIteration()
{
    std::stringstream outputDirectoryPath;

    // PROJECT_DIR is defined in the CMakeList.txt file for this library
    // It is an easy and consistent way to get Root file path
    outputDirectoryPath << PROJECT_DIR;
    outputDirectoryPath << "/" << OUTPUT_DIR_NAME;

    // If directory DOES NOT exist, create it
    if( !(std::filesystem::is_directory(outputDirectoryPath.str())) )
    {   
        if( !(std::filesystem::create_directory(outputDirectoryPath.str())) )
        {
            throw std::filesystem::filesystem_error("Unable to create output Directory", std::error_code());
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
    {
        throw std::filesystem::filesystem_error("ERROR", std::error_code());
    }

    // Set the Output Directory in the LogRegistry
    Utility::LogRegistry::instance()->configureRegistry(mOutputDirPath);

    // Initialize the Global Console Logger
    initializeGlobalLogger();

    // Get GlobalLogger
    auto cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    std::stringstream logStream;
    logStream << "Initialized Output Directory: " << mOutputDirPath;
    cLogger->logInfo(logStream.str());

}

void Core::Configuration::initializeGlobalLogger()
{
    // Get Global Logger
    std::shared_ptr< Utility::Logger > cLogger = Utility::LogRegistry::instance()->getGlobalLogger();

    if(cLogger)
    {
        auto globalConsoleSink = std::make_shared< Utility::ColorConsoleSink >();
        cLogger->addSink(globalConsoleSink);
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