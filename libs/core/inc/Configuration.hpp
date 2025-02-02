#pragma once

#include "core/inc/ConfigurationI.hpp"
#include "utility/inc/Logging/Logger.hpp"
#include "utility/inc/Logging/Sinks/ColorConsoleSink.hpp"

#include "core/inc/IniParser.hpp"

#include <string>
#include <map>

namespace Core
{
    /*
        Configuration sets up the Config Directory, Output Directory, and contains the parsers that will parse config files.
        The Configuration class is responsible for handing each parser the correct file based on its file extension.
        Files that lack extensions are ignored
    */
    class Configuration : public ConfigurationI
    {
        public:
            Configuration(const std::string& configDirectory);

            bool initializeIteration() override;

            const std::string getOutDirPath();

            ~Configuration() {}

        private:
            void initializeParsers();

            bool initializeConfigDirectory();
            bool initializeOutputDirectory();

            void initializeGlobalLogger();

        private:
            // FilePath information
            std::string mConfigDirPath;
            std::string mConfigDirName;

            std::string mOutputDirPath;
            static const std::string OUTPUT_DIR_NAME;

            // Key is the string name identifier of parser
            std::map< std::string, std::unique_ptr<Core::ParserI> > mConfigParserMap;

            std::string mProjectDirectory;
    };
}