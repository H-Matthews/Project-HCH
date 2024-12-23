#pragma once

#include "utility/inc/Logging/Logger.hpp"

#include <functional> // Hash
#include <map>
#include <memory>

namespace Utility 
{

    /*
        Singleton Class Design
        Contains a registry of loggers. Once a logger is registered, it can be retrieved with getLogger
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

            const std::string getAppOutputDir() const;

        private:
            LogRegistry();

            bool checkForTextSink(std::shared_ptr< Logger > logger);

            // Static pointer to our object
            static std::shared_ptr< LogRegistry > mRegistryInstance;

            // Key is Hashed String (Logger Name)
            std::map< std::size_t, std::shared_ptr< Logger > > mRegistry;

            // Hash Function
            std::hash<std::string> mHash;

            std::string mAppOutputDirectory;
    };
}