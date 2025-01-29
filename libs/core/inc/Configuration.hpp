#pragma once

#include "core/inc/ConfigurationI.hpp"
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

#include <string>

namespace Core
{
    /*
        This Class will setup all of the information that our Application requires to run
        
    */
    class Configuration : public ConfigurationI
    {
        public:
            Configuration();

            bool initializeIteration() override;
            void loadSettings() override;

            const std::string getOutDirPath();

            ~Configuration() {}

        private:
            bool initializeOutputDirectory();
            bool initializeConfigDirectory();

            void initializeGlobalLogger();

        private:
            // FilePath information
            std::string mConfigDirPath;
            static const std::string CONFIG_DIR_NAME;

            std::string mOutputDirPath;
            static const std::string OUTPUT_DIR_NAME;

            std::string mProjectDirectory;
    };
}