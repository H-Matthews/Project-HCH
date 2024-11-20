#pragma once

#include <functional> // Hash
#include <map>
#include <memory>
/*
namespace Utility 
{

    // Singleton Class Design
    class LogRegistry
    {
        public:
            // Delete the Copy Constructor
            LogRegistry(const LogRegistry& obj) = delete;

            // GetInstance
            static std::shared_ptr< LogRegistry > getInstance();

            // Configuration 
            void configureRegistry(const std::string outputDirPath);

            // Create TextFile Logger with fileName
            void registerLogger(const std::string& fileName);

            // Get TextFile Logger
            std::shared_ptr< TextFileLogger > getLogger(const std::string& fileName);

        private:
            LogRegistry();

            // Static pointer to our object
            static std::shared_ptr< LogRegistry > mLogInstance;

            // Key is Hashed String (Logger Name)
            std::map< std::size_t, std::shared_ptr< TextFileLogger > > mRegistry;

            std::string mOutDirPath;
            std::unique_ptr<ConsoleLogger> mCLogger;
    };
}
*/