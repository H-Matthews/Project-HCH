#include "utility/Logging/LogRegistry.hpp"

#include <sstream>

std::shared_ptr<Utility::LogRegistry> Utility::LogRegistry::mRegistryInstance = nullptr;

Utility::LogRegistry::LogRegistry() : mRegistry(), mHash(), mOutputDirectory("") {}

std::shared_ptr<Utility::LogRegistry> Utility::LogRegistry::instance() {
    if (mRegistryInstance == nullptr)
        mRegistryInstance = std::shared_ptr<Utility::LogRegistry>(new Utility::LogRegistry());

    return mRegistryInstance;
}

void Utility::LogRegistry::configureRegistry(const std::string outputDirPath) {
    mOutputDirectory = outputDirPath;
}

// Creates a logger. The Key is the hashed logger name
void Utility::LogRegistry::registerLogger(std::shared_ptr<Utility::Logger> logger) {
    if (!logger)
        return;

    std::size_t key = mHash(logger->getLoggerName());
    if (mRegistry.contains(key)) {
        if (auto cLogger = getGlobalLogger())
            cLogger->logWarn("Logger name collision on registration: " + logger->getLoggerName());
    }
    mRegistry.insert_or_assign(key, logger);
}

// IF the logger is NOT FOUND, it returns a nullptr
std::shared_ptr<Utility::Logger> Utility::LogRegistry::getLogger(const std::string& fileName) {
    std::size_t hashedString = mHash(fileName);
    auto it = mRegistry.find(hashedString);
    if (it == mRegistry.end())
        return nullptr;
    return it->second;
}

// IF the global logger is NOT FOUND, it returns a nullptr
std::shared_ptr<Utility::Logger> Utility::LogRegistry::getGlobalLogger() const {
    // Look for the global Logger in mRegistry
    std::shared_ptr<Utility::Logger> globalLogger = nullptr;
    for (const auto& [hashKey, logger] : mRegistry) {
        if (logger->getIsGlobalLogger())
            globalLogger = logger;
    }

    return globalLogger;
}

const std::string Utility::LogRegistry::getOutputDir() const {
    return mOutputDirectory;
}