#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include <iostream>

// Parameters that are in EVERY toml config file
const std::string Core::TOMLConfigReader::PARAM_FILE_TYPE = "FILE_TYPE";

void Core::TOMLConfigReader::init()
{
    return;
}

void Core::TOMLConfigReader::readFile( const std::filesystem::path& filePath, ConfigNode& configNode )
{
    // Check if file exists
    if ( ( !std::filesystem::is_regular_file( filePath ) ) )
    {
        const std::string errString = "Could NOT FIND file ----> " + filePath.string();
        configNode.retStatus = std::make_pair( false, errString );

        return;
    }

    toml::table config;
    try
    {
        config = toml::parse_file( filePath.string() );
    }
    catch ( const toml::parse_error& err )
    {
        configNode.retStatus = std::make_pair( false, err.what() );
        return;
    }

    // 1. GET FILE_TYPE
    std::string fileType;
    auto retPair = extractTOMLString( config, PARAM_FILE_TYPE, fileType );
    if ( !retPair.first )
    {
        configNode.retStatus = std::make_pair( retPair.first, retPair.second + ", FILE: " + filePath.string() );
        return;
    }

    ConfigFileID fileID = stringToEnum( fileType );
    if ( fileID == ConfigFileID::SIZE )
    {
        configNode.retStatus =
            std::make_pair( false, "PARAMETER: " + PARAM_FILE_TYPE + " COULD NOT BE MAPPED TO A CONFIG_FILE_ID" +
                                       "FILE: " + filePath.string() );
    }

    // SET FILEID
    configNode.configFileID = fileID;

    switch ( fileID )
    {
        case ConfigFileID::ROOT:
        {
            auto rootConfigType = std::make_shared< RootConfigType >();
            auto retPair = handleRootFile( config, rootConfigType );
            if ( !retPair.first )
            {
                configNode.retStatus = retPair;

                return;
            }

            configNode.configType = rootConfigType;

            break;
        }
        case ConfigFileID::CORE_CONFIGURABLES:
        {
            // handleCoreConfigFile( config );

            break;
        }
        case ConfigFileID::PREFABS:
        {
            // FUTURE

            break;
        }
        case ConfigFileID::SIZE:
        {
            // DO NOTHING

            break;
        }
    }

    configNode.retStatus = std::make_pair( true, "" );

    return;
}

std::pair< bool, std::string > Core::TOMLConfigReader::handleRootFile(
    const toml::table& configTable, std::shared_ptr< RootConfigType > rootConfigType )
{
    // ITERATE OVER THE DEFINED TABLE KEYS
    for ( const auto& key : rootConfigType->absoluteTablePaths )
    {
        auto nodeView = configTable.at_path( key );

        auto tomlStringView = nodeView.as_string();
        if ( tomlStringView )
        {
            std::string myValue = tomlStringView->value_or( "" );
            if ( !myValue.empty() )
                rootConfigType->configFiles.push_back( myValue );
        }
    }

    return std::make_pair( true, "" );
}

// std::pair< bool, std::string > Core::TOMLConfigReader::handleCoreConfigFile(
//     const toml::table& configTable, std::shared_ptr< ConfigNode > configNode )
// {}

// BEGIN HELPER FUNCTIONS ------------------------------------------------------------------------------

std::pair< bool, std::string > Core::TOMLConfigReader::extractTOMLString(
    const toml::table& configTable, const std::string& stringParameter, std::string& extractedString )
{
    // 1. DETERMINE FILE_TYPE
    auto it = configTable.find( stringParameter );
    if ( it == configTable.end() )
        return std::make_pair( false, "PARAMETER: " + PARAM_FILE_TYPE + ", IS MISSING" );

    auto tomlFileString = it->second.as_string();
    if ( !tomlFileString )
        return std::make_pair( false, "PARAMETER: " + PARAM_FILE_TYPE + ", VALUE IS NOT A STRING" );

    const std::string fileTypeString = tomlFileString->value_or( "" );
    if ( fileTypeString.empty() )
        return std::make_pair( false, "PARAMETER: " + PARAM_FILE_TYPE + ", HAS AN EMPTY VALUE" );

    extractedString = fileTypeString;

    return std::make_pair( true, "" );
}