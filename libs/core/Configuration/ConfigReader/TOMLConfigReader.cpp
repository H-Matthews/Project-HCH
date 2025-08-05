#include "core/Configuration/ConfigReader/TOMLConfigReader.hpp"

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <iostream>
#include <limits>

// Parameters that are in EVERY toml config file
const std::string Core::TOMLConfigReader::PARAM_FILE_TYPE = "FILE_TYPE";

void Core::TOMLConfigReader::init()
{
    return;
}

std::pair< bool, std::string > Core::TOMLConfigReader::readFile(
    const std::filesystem::path& filePath, std::shared_ptr< ConfigNode >& configNode )
{
    // Check if file exists
    if ( ( !std::filesystem::is_regular_file( filePath ) ) )
    {
        const std::string errString = "Could NOT FIND file ----> " + filePath.string();

        return std::make_pair( false, errString );
    }

    if ( !configNode )
        configNode = std::make_shared< ConfigNode >( filePath.filename().stem().string() );

    toml::table config;
    try
    {
        config = toml::parse_file( filePath.string() );
    }
    catch ( const toml::parse_error& err )
    {
        return std::make_pair( false, err.what() );
    }

    // BEGIN PARSING TOML DATA TO CONFIG TREE-----------------------------------------------------------
    processTOMLData( config, configNode );

    return std::make_pair( true, "" );
}

void Core::TOMLConfigReader::processTOMLData( const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode )
{

    // Determine Node Type
    if ( tomlNode.is_table() )
    {
        const auto& tomlTable = tomlNode.as_table();
        for ( auto&& [ key, value ] : *tomlTable )
        {
            std::string keyString( key.str() );

            // IF its a table, then we need to recursively call this function
            if ( value.is_table() || value.is_array() )
            {
                auto newConfigNode = std::make_shared< ConfigNode >( keyString, configNode );
                configNode->addChild( newConfigNode );
                processTOMLData( value, newConfigNode );
            }
            else // For arrays, we must iterate over all elements
            {
                processPrimitiveTOMLData( keyString, value, configNode );
            }
        }
    }
    else if ( tomlNode.is_array() )
    {
        const auto& tomlArray = tomlNode.as_array();
        for ( auto&& val : *tomlArray )
        {
            processArrayTOMLData( val, configNode );
        }
    }

    return;
}

// BEGIN HELPER FUNCTIONS ------------------------------------------------------------------------------

void Core::TOMLConfigReader::processArrayTOMLData(
    const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode )
{

    if ( tomlNode.is_string() )
    {
        configNode->insertArrayValue( tomlNode.as_string()->value_or( "" ) );
    }
    else if ( tomlNode.is_integer() )
    {
        configNode->insertArrayValue( tomlNode.as_integer()->value_or( std::numeric_limits< int >::max() ) );
    }
    else if ( tomlNode.is_floating_point() )
    {
        configNode->insertArrayValue( tomlNode.as_floating_point()->value_or( std::numeric_limits< double >::max() ) );
    }
    else if ( tomlNode.is_boolean() )
    {
        configNode->insertArrayValue( tomlNode.as_boolean()->value_or( false ) );
    }

    return;
}

void Core::TOMLConfigReader::processPrimitiveTOMLData(
    const std::string& keyString, const toml::node& tomlNode, std::shared_ptr< ConfigNode > configNode )
{

    if ( tomlNode.is_string() )
    {
        configNode->insertValuePair( keyString, tomlNode.as_string()->value_or( "" ) );
    }
    else if ( tomlNode.is_integer() )
    {
        configNode->insertValuePair( keyString, tomlNode.as_integer()->value_or( std::numeric_limits< int >::max() ) );
    }
    else if ( tomlNode.is_floating_point() )
    {
        configNode->insertValuePair(
            keyString, tomlNode.as_floating_point()->value_or( std::numeric_limits< double >::max() ) );
    }
    else if ( tomlNode.is_boolean() )
    {
        configNode->insertValuePair( keyString, tomlNode.as_boolean()->value_or( false ) );
    }

    return;
}