#include "utility/inc/Logging/LogRegistry.hpp"
#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include<iostream>
#include<sstream>

namespace Utility
{
    std::shared_ptr< LogRegistry > LogRegistry::mRegistryInstance = nullptr;

    LogRegistry::LogRegistry() :
        mRegistry(),
        mAppOutputDirectory("")
    {
    }

    std::shared_ptr< LogRegistry > LogRegistry::instance()
    {
        if(mRegistryInstance == nullptr)
            mRegistryInstance = std::shared_ptr< LogRegistry >(new LogRegistry());

        return mRegistryInstance;
    }

    void LogRegistry::configureRegistry(const std::string outputDirPath)
    {
        mAppOutputDirectory = outputDirPath;
    }

    // Create TextFile Logger with fileName
    void LogRegistry::registerLogger(std::shared_ptr< Logger > logger)
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

    bool LogRegistry::checkForTextSink(std::shared_ptr< Logger > logger)
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

    std::shared_ptr< Logger > LogRegistry::getLogger(const std::string& fileName)
    {
        std::shared_ptr< Logger > textFileLogger = nullptr;

        // Hash String
        std::size_t hashedString = mHash(fileName);

        // Look for hashedString in registry
        std::map< std::size_t, std::shared_ptr< Logger > >::iterator it;
        it = mRegistry.find(hashedString);

        textFileLogger = (*it).second;
        if(textFileLogger == nullptr)
        {
            std::cout << "The logger name was NOT found in the Logger Registry! "
                    << "Logger Name: " << fileName << std::endl;
        }
        
        return textFileLogger;
    }

    const std::string LogRegistry::getAppOutputDir() const
    {   
        return mAppOutputDirectory;
    }
}