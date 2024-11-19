#pragma once

#include "core/inc/ConfigurationI.hpp"

#include "utility/inc/ConsoleLogger.hpp"
#include "utility/inc/TextFileLogger.hpp"

#include <string>
#include <memory>

namespace Core
{
    class Configuration : public ConfigurationI
    {
        public:
            Configuration();

            void initializeIteration() override;
            void loadSettings() override;

            const std::string getOutDirPath();

        private:
            // FilePath information
            std::string mConfigDirPath;
            static const std::string CONFIG_DIR_NAME;
            std::string mOutputDirPath;
            static const std::string OUTPUT_DIR_NAME;

            // Logging
            std::unique_ptr<Utility::ConsoleLogger> mCLogger;
    };
}