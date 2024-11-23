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
        bool hasTextSink = checkForTextSink(logger);

        if(hasTextSink)
        {
            // Ensure output directory is set
            if(mAppOutputDirectory != "")
                mRegistry.insert(std::make_pair(hashedString, logger));
        }
        else
        {
            mRegistry.insert(std::make_pair(hashedString, logger));
        }

        auto cLogger = Utility::LogRegistry::instance()->getLogger("cLogger");
        if(cLogger)
        {
            std::stringstream logStream;
            logStream << "Registered Logger: " << logger->getLoggerName();
            logStream << " at Global Loglevel: " << logger->getGlobalLogLevelAsString();

            cLogger->logInfo(logStream.str());
        }
    }
}

bool Utility::LogRegistry::checkForTextSink(std::shared_ptr< Utility::Logger > logger)
{
    bool hasTextSink = false;

    auto sinks = logger->getSinkReferences();
    if(sinks.size() > 0 )
    {
        for(const auto& sink : sinks)
        {
            auto textSink = dynamic_cast< Utility::TextFileSink* > (sink);
            if(textSink != nullptr)
                hasTextSink = true;

            if(hasTextSink == true)
                break;
        }
    }

    return hasTextSink;
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

const std::string Utility::LogRegistry::getAppOutputDir() const
{   
    return mAppOutputDirectory;
}