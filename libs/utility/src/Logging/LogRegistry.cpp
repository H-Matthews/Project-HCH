#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

std::shared_ptr< Utility::LogRegistry > Utility::LogRegistry::mRegistryInstance = nullptr;

Utility::LogRegistry::LogRegistry() :
    mRegistry(),
    mAppOutputDirectory("")
{
}

std::shared_ptr< Utility::LogRegistry > Utility::LogRegistry::instance()
{
    if(mRegistryInstance == nullptr)
        mRegistryInstance = std::shared_ptr< Utility::LogRegistry >(new Utility::LogRegistry());

    return mRegistryInstance;
}

void Utility::LogRegistry::configureRegistry(const std::string outputDirPath)
{
    mAppOutputDirectory = outputDirPath;
}

// Create TextFile Logger with fileName
void Utility::LogRegistry::registerLogger(std::shared_ptr< Utility::Logger > logger)
{
    if(logger != nullptr)
    {
        // Hash String
        std::size_t hashedString = mHash(logger->getLoggerName());
        mRegistry.insert(std::make_pair(hashedString, logger));

        auto cLogger = this->getGlobalLogger();
        if(cLogger)
        {
            std::stringstream logStream;
            logStream << "Registered Logger: " << logger->getLoggerName();
            logStream << " at Global Loglevel: " << logger->getGlobalLogLevelAsString();

            cLogger->logInfo(logStream.str());
        }
        
    }
}

std::shared_ptr< Utility::Logger > Utility::LogRegistry::getLogger(const std::string& fileName)
{
    std::shared_ptr< Logger > textFileLogger = nullptr;

    // Hash String
    std::size_t hashedString = mHash(fileName);

    // Look for hashedString in registry
    std::map< std::size_t, std::shared_ptr< Utility::Logger > >::iterator it;
    it = mRegistry.find(hashedString);

    textFileLogger = (*it).second;
    
    return textFileLogger;
}

std::shared_ptr< Utility::Logger > Utility::LogRegistry::getGlobalLogger()
{
    // Look for the global Logger in mRegistry
    std::shared_ptr < Utility::Logger > globalLogger = nullptr;
    for(const auto& [hashKey, logger] : mRegistry)
    {
        if(logger->getIsGlobalLogger())
        {
            globalLogger = logger;
        }
    }

    return globalLogger;
}

const std::string Utility::LogRegistry::getAppOutputDir() const
{   
    return mAppOutputDirectory;
}