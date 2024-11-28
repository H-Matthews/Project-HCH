#pragma once

#include "utility/inc/Logging/Logger.hpp"

#include <functional> // Hash
#include <map>
#include <memory>

namespace Utility 
{

    /*
        Singleton Class Design
        Defines a registry of loggers.
    */
    class LogRegistry
    {
        public:
            // Delete the Copy Constructor
            LogRegistry(const LogRegistry& obj) = delete;

            // GetInstance
            static std::shared_ptr< LogRegistry > instance();

            // Configure the Applications output directory 
            void configureRegistry(const std::string outputDirPath);

            // Associate Logger with hashed value, store in mRegistry
            void registerLogger(std::shared_ptr< Logger > logger);

            // If the Logger was NOT found, it returns a nullptr
            std::shared_ptr< Logger > getLogger(const std::string& fileName);

            std::shared_ptr< Logger > getGlobalLogger() const;

            const std::string getAppOutputDir() const;

        private:
            LogRegistry();

            // Static pointer to our object
            static std::shared_ptr< LogRegistry > mRegistryInstance;

            // Key is Hashed String (Logger Name)
            std::map< std::size_t, std::shared_ptr< Logger > > mRegistry;

            // Hash Function
            std::hash<std::string> mHash;

            std::string mAppOutputDirectory;
    };
}