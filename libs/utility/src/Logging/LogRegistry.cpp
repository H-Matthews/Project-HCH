#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

std::shared_ptr< Utility::LogRegistry > Utility::LogRegistry::mRegistryInstance = nullptr;

Utility::LogRegistry::LogRegistry() :
    mRegistry(),
    mHash(),
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

// Creates a logger. The Key is the hashed logger name
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

// IF the logger is NOT FOUND, it returns a nullptr
std::shared_ptr< Utility::Logger > Utility::LogRegistry::getLogger(const std::string& fileName)
{
    std::shared_ptr< Logger > logger = nullptr;

    std::size_t hashedString = mHash(fileName);

    // Find Logger
    std::map< std::size_t, std::shared_ptr< Utility::Logger > >::iterator it;
    it = mRegistry.find(hashedString);

    logger = (*it).second;
    
    return logger;
}

// IF the global logger is NOT FOUND, it returns a nullptr
std::shared_ptr< Utility::Logger > Utility::LogRegistry::getGlobalLogger() const
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