#include "core/Configuration/Configuration.hpp"

#include <filesystem>

Core::Configuration::Configuration( ConfigSpec configSpec ) :
    mConfigDirPath( std::string( PROJECT_DIR ) + "/" + configSpec.configDirectory ),
    mRootFile( std::move( configSpec.rootConfigFile ) ),
    mConfigReader( std::move( configSpec.configReader ) ),
    mConfigFiles(),
    mOwnedSections(),
    mIndex(),
    mSectionSource()
{
    if (!( std::filesystem::is_directory( mConfigDirPath ) ))
        throw ConfigurationException( ( "Config directory could NOT be found: " + mConfigDirPath ).c_str() );

    if (mRootFile.empty())
        throw ConfigurationException( "Root file was NOT populated" );

    if (!mConfigReader)
        throw ConfigurationException( "ConfigReader is NULL" );

    parse();
}

void Core::Configuration::parse()
{
    auto rootSection = parseRootFile();
    auto childSections = parseConfigFiles();

    indexFile( *rootSection, mConfigDirPath + "/" + mRootFile );
    for (std::size_t i = 0; i < childSections.size(); ++i)
        indexFile( *childSections[ i ], mConfigFiles[ i ] );
}

std::unique_ptr< Core::ConfigSection > Core::Configuration::parseRootFile()
{
    const std::string rootFilePath = mConfigDirPath + "/" + mRootFile;

    auto rootSection = mConfigReader->readFile( std::filesystem::path( rootFilePath ) );
    if (!rootSection)
        throw ConfigurationException( ( "Could not find root config file: " + rootFilePath ).c_str() );

    auto filesSection = rootSection->getSection( RootFilesSection::NAME );
    if (filesSection)
    {
        if (auto coreConfigFile = filesSection->getString( RootFilesSection::CORE_CONFIGURABLES ))
            mConfigFiles.push_back( mConfigDirPath + "/" + *coreConfigFile );

        if (auto prefabConfigFile = filesSection->getString( RootFilesSection::PREFABS ))
            mConfigFiles.push_back( mConfigDirPath + "/" + *prefabConfigFile );
    }

    return rootSection;
}

std::vector< std::unique_ptr< Core::ConfigSection > > Core::Configuration::parseConfigFiles()
{
    std::vector< std::unique_ptr< ConfigSection > > sections;
    sections.reserve( mConfigFiles.size() );

    for (const auto& configFile : mConfigFiles)
    {
        auto section = mConfigReader->readFile( std::filesystem::path( configFile ) );
        if (!section)
            throw ConfigurationException( ( "Could not find config file: " + configFile ).c_str() );

        sections.push_back( std::move( section ) );
    }

    return sections;
}

void Core::Configuration::indexFile( const ConfigSection& fileRoot, const std::string& sourcePath )
{
    for (const auto& sectionName : fileRoot.sectionNames())
    {
        auto existing = mSectionSource.find( sectionName );
        if (existing != mSectionSource.end())
        {
            throw ConfigurationException(
                ( "Duplicate config section [" + sectionName + "] declared in " + sourcePath +
                  "; first declared in " + existing->second )
                    .c_str() );
        }

        auto owned = fileRoot.getSection( sectionName );
        if (!owned)
            continue;

        mIndex.emplace( sectionName, owned.get() );
        mSectionSource.emplace( sectionName, sourcePath );
        mOwnedSections.push_back( std::move( owned ) );
    }
}

const Core::ConfigSection* Core::Configuration::getSection( std::string_view name ) const
{
    auto it = mIndex.find( name );
    return it != mIndex.end() ? it->second : nullptr;
}
