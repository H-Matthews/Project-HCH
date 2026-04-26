#pragma once

#include "core/Exceptions/ConfigurationException.hpp"
#include "core/Configuration/ConfigReader/ConfigReader.hpp"
#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Core
{
    /**
     * Startup parameters passed to Configuration. Owns the ConfigReader implementation
     * so the caller decides which file format is used without touching Configuration itself.
     */
    struct ConfigSpec
    {
        std::string rootConfigFile;
        std::string configDirectory;

        std::unique_ptr< ConfigReader > configReader;

        ConfigSpec() = default;
        ConfigSpec( const ConfigSpec& ) = delete;
        ConfigSpec& operator=( const ConfigSpec& ) = delete;
        ConfigSpec( ConfigSpec&& ) = default;
        ConfigSpec& operator=( ConfigSpec&& ) = default;
    };

    /**
     * Parses the project's config files and serves their contents to subsystems
     * as scoped ConfigSection objects
     */
    class Configuration
    {
      public:
        static constexpr std::string_view SECTION_NAME = "Configuration";

        explicit Configuration( ConfigSpec configSpec );
        ~Configuration() = default;

        /**
         * Returns a non-owning pointer to the named top-level section, or nullptr
         * if the section is not present. The returned pointer is valid for the
         * lifetime of this Configuration object.
         */
        const ConfigSection* getSection( std::string_view name ) const;

      private:
        void parse();
        std::unique_ptr< ConfigSection > parseRootFile();
        std::vector< std::unique_ptr< ConfigSection > > parseConfigFiles();

        void indexFile( const ConfigSection& fileRoot, const std::string& sourcePath,
                        std::map< std::string, std::string >& sectionSource );

        std::string mConfigDirPath;
        std::string mRootFile;

        std::unique_ptr< ConfigReader > mConfigReader;

        std::vector< std::string > mConfigFiles;

        std::vector< std::unique_ptr< ConfigSection > > mOwnedSections;
        std::map< std::string, const ConfigSection*, std::less<> > mIndex;

        struct RootFilesSection
        {
            static constexpr std::string_view NAME = "Configuration_Files";
            static constexpr std::string_view FILES = "files";
        };
    };
}
