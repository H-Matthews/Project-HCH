#pragma once

#include "utility/Logging/Logger.hpp"

#include <functional>
#include <map>
#include <memory>

namespace Utility
{

    class Logger;

    /**
     * LogRegistry is a Singleton that defines a registry of loggers
     * When creating a core Logger, it should be registered into this registry
     *
     * NOTE: There is a Global Logger defined in the registry that is used for printing output
     * to the Console when debugging. This should be utilized over using std::cout, due to formatting benefits
     */
    class LogRegistry
    {
      public:
        // Delete the Copy Constructor & Assignment operator
        LogRegistry( const LogRegistry& obj ) = delete;
        LogRegistry& operator=( const LogRegistry& ) = delete;

        static std::shared_ptr< LogRegistry > instance();

        // Configure the output directory
        void configureRegistry( const std::string outputDirPath );

        // Associate Logger with hashed value, store in mRegistry
        void registerLogger( std::shared_ptr< Logger > logger );

        // If the Logger was NOT found, it returns a nullptr
        std::shared_ptr< Logger > getLogger( const std::string& fileName );

        std::shared_ptr< Logger > getGlobalLogger() const;

        const std::string getOutputDir() const;

      private:
        LogRegistry();

        // Static pointer to our object
        static std::shared_ptr< LogRegistry > mRegistryInstance;

        // Key is Hashed String (Logger Name)
        std::map< std::size_t, std::shared_ptr< Logger > > mRegistry;

        // Hash Function
        std::hash< std::string > mHash;

        std::string mOutputDirectory;
    };
}