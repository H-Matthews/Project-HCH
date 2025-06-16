#pragma once

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"
#include "utility/Logging/LogRegistry.hpp"

#include "core/Configuration/ConfigurationI.hpp"

#include "core/Configuration/Parsers/IniParser.hpp"
#include "core/Configuration/Parsers/ParserTypes.hpp"

#include <string>
#include <functional>
#include <map>
#include <vector>

namespace Core
{
    struct FileInformation
    {
        std::string mFileName;
        std::string mFileExtension;

        FileInformation( std::string fileName, std::string fileExtension ) :
            mFileName( fileName ),
            mFileExtension( fileExtension )
        {}
    };

    /**
     * Configuration sets up the Config Directory, Output Directory, Game asset file paths, and contains the parsers
     * that will parse config files. The Configuration class is responsible for handing each parser the correct file
     * stream based on its file extension. Files that lack extensions are ignored
     */
    class Configuration : public ConfigurationI
    {
      public:
        Configuration();

        void initializeIteration() override;

        void parseConfigs() override;

        // TODO This needs to be removed. Shouldn't have an interface function as a getter like this
        std::pair< std::string, std::string > getAssetPaths() override;

        const std::string getOutDirPath();

        ~Configuration()
        {}

      private:
        void initializeParsers();

        // Needs to be a template so that we can treat registerParser as a factory
        template < typename T >
        void registerParser( Parsers::ID parserID );

        std::unique_ptr< Core::Parser > createParser( Parsers::ID parserID );

        void initializeConfigDirectory();
        void initializeConfigFiles();

        void initializeOutputDirectory();

        void initializeAssetDirectorys();

        void initializeGlobalLogger();

      private:
        // FilePath information
        std::string mConfigDirPath;
        static const std::string CONFIG_DIR_NAME;

        std::string mOutputDirPath;
        static const std::string OUTPUT_DIR_NAME;

        std::string mAssetDirPath;
        std::string mAssetFontsDirPath;
        std::string mAssetTexturesDirPath;
        static const std::string ASSET_DIR_NAME;
        static const std::string ASSET_FONTS_DIR_NAME;
        static const std::string ASSET_TEXTURES_DIR_NAME;

        std::vector< Core::FileInformation > mConfigFiles;
        std::map< std::string, Parsers::ID > mParserFileExtensionToIDMap;

        // This contains a function that ALLOCATES memory for a specified Parser object
        // This way we ONLY create a parser if we have a config file that uses it
        std::map< Parsers::ID, std::function< std::unique_ptr< Core::Parser >() > > mParserRegistry;

        // Holds the actual pointer to the Parser object
        std::map< Parsers::ID, std::unique_ptr< Core::Parser > > mParsers;

        std::string mProjectDirectory;
    };
}

template < typename T >
void Core::Configuration::registerParser( Parsers::ID parserID )
{
    const std::string identifierString( Parsers::parserEnumsToString( parserID ) );

    // Insert into file extension MAP
    mParserFileExtensionToIDMap[ identifierString ] = parserID;

    // Stores a Lambda in mParserRegistry
    mParserRegistry[ parserID ] = [ identifierString ]()
    { return std::unique_ptr< Parser >( new T( identifierString ) ); };

    if constexpr ( Utility::CAN_LOG )
    {
        std::string logMessage;
        logMessage = "Registered Parser: " + identifierString;
        Utility::LogRegistry::instance()->getGlobalLogger()->logInfo( logMessage );
    }
}