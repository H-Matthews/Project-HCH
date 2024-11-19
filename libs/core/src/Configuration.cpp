#include "core/inc/Configuration.hpp"

#include "utility/inc/LogRegistry.hpp"
#include "utility/inc/ConsoleLogger.hpp"

#include <sstream>
#include <filesystem>
#include <iostream>


namespace Core 
{
    const std::string Configuration::CONFIG_DIR_NAME = "configs";
    const std::string Configuration::OUTPUT_DIR_NAME = "output";

    Configuration::Configuration() :
        mConfigDirPath(),
        mOutputDirPath(),
        mCLogger(std::make_unique<Utility::ConsoleLogger>())
    {
    }

    void Configuration::initializeIteration()
    {
        // Build our Path to the output directory
        std::stringstream outputDirectoryPath;

        // PROJECT_DIR is defined in the CMakeList.txt file for this library
        // It is an easy and consistent way to get Root file path
        outputDirectoryPath << PROJECT_DIR;
        outputDirectoryPath << "/" << OUTPUT_DIR_NAME;
        std::cout << outputDirectoryPath.str() << std::endl;

        // Check to see if Directory exists
        if(std::filesystem::is_directory(outputDirectoryPath.str()))
        {
            std::stringstream logMessage;
            logMessage << "Output Directory Already Exists ";
            logMessage << "Path:" << outputDirectoryPath.str();

            LOG_INFO(mCLogger, logMessage.str());
        }
        else // IF NOT, create it
        {
            if( std::filesystem::create_directory(outputDirectoryPath.str()) )
            {
                std::stringstream logMessage;
                logMessage << "SUCCESSFULLY created output directory ";
                logMessage << "Path:" << outputDirectoryPath.str();

                LOG_INFO(mCLogger, logMessage.str());
            }
            else
            {
                throw std::filesystem::filesystem_error("Unable to create Output Directory", std::error_code());
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
        if( std::filesystem::create_directory(outputDirectoryPath.str()) )
        {
            std::stringstream logMessage;
            logMessage << "SUCCESSFULLY created run directory ";
            logMessage << "Path:" << outputDirectoryPath.str();

            // Set the Output Directory in the LogRegistry
            Utility::LogRegistry::getInstance()->configureRegistry(mOutputDirPath);

            LOG_INFO(mCLogger, logMessage.str());
        }
        else
        {
            throw std::filesystem::filesystem_error("ERROR", std::error_code());
        }

    }

    void Configuration::loadSettings()
    {
        // TODO: Define how we will read data into the program (ini?, JSON?)
    }

    const std::string Configuration::getOutDirPath()
    {
        return mOutputDirPath;
    }

}