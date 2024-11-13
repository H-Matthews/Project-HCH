#pragma once

#include "core/inc/ConfigurationI.hpp"

#include "utility/inc/ConsoleLogger.hpp"

#include <string>
namespace Core
{
    class Configuration : public ConfigurationI
    {
        public:
            Configuration();

            void initializeIteration() override;
            void loadSettings() override;

        private:
            // FilePath information
            std::string mConfigDirPath;
            static const std::string CONFIG_DIR_NAME;
            std::string mOutputDirPath;
            static const std::string OUTPUT_DIR_NAME;

            // Logging
            Utility::ConsoleLogger mCLogger;
    };
}