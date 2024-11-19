#include "utility/inc/LogRegistry.hpp"

#include<sstream>

namespace Utility
{
    std::shared_ptr< LogRegistry > LogRegistry::mLogInstance = nullptr;

    LogRegistry::LogRegistry() :
        mRegistry(),
        mOutDirPath(""),
        mCLogger(std::make_unique<ConsoleLogger>())
    {
    }

    std::shared_ptr< LogRegistry > LogRegistry::getInstance()
    {
        if(mLogInstance == nullptr)
            mLogInstance = std::shared_ptr< LogRegistry >(new LogRegistry());

        return mLogInstance;
    }

    void LogRegistry::configureRegistry(const std::string outputDirPath)
    {
        mOutDirPath = outputDirPath;
    }

    // Create TextFile Logger with fileName
    void LogRegistry::registerLogger(const std::string& fileName)
    {
        std::stringstream logStream;

        // Ensure output directory is set
        if(mOutDirPath != "")
        {
            // Hash String
            std::hash<std::string> hash;
            std::size_t hashedString = hash(fileName);

            // Create instance and insert into loggers
            auto loggerInstance = std::make_shared< TextFileLogger >(fileName, mOutDirPath);
            auto ret = mRegistry.insert(std::make_pair(hashedString, loggerInstance));
            
            if(ret.second)
            {
                LOG_INFO(mCLogger, "Successfully Registered TextFile Logger Instance: " + fileName);
            }
            else 
            {
                LOG_ERROR(mCLogger, "FAILED to Create TextFile Logger Instance: " + fileName);
            }
        }
        else 
        {
            LOG_ERROR(mCLogger, "FAILED to create TextFile Logger Instance: " + fileName);
        }
    }

    std::shared_ptr< TextFileLogger > LogRegistry::getLogger(const std::string& fileName)
    {
        std::shared_ptr< TextFileLogger > textFileLogger = nullptr;
        std::stringstream sStream;

        // Hash String
        std::hash<std::string> hash;
        std::size_t hashedString = hash(fileName);

        // Look for hashedString in registry
        std::map< std::size_t, std::shared_ptr< TextFileLogger > >::iterator it;
        it = mRegistry.find(hashedString);

        textFileLogger = (*it).second;
        if(textFileLogger == nullptr)
        {
            sStream << "The logger name was NOT found in the Logger Registry! "
                    << "Logger Name: " << fileName << std::endl;
            LOG_ERROR(mCLogger, sStream.str());
        }
        
        return textFileLogger;
    }
}