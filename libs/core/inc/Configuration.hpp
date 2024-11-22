#pragma once

#include "core/inc/ConfigurationI.hpp"
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

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
            const std::string mGlobalCLoggerName;
            std::shared_ptr< Utility::Logger > mGlobalCLogger;

            // FilePath information
            std::string mConfigDirPath;
            static const std::string CONFIG_DIR_NAME;
            std::string mOutputDirPath;
            static const std::string OUTPUT_DIR_NAME;
    };
}