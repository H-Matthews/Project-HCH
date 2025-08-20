#pragma once

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "utility/StringOperations.hpp"

#include <optional>
#include <string>
#include <memory>

namespace Core
{

    /**
     * Utility functions for the ConfigurationTree lookup / traversal
     */
    namespace ConfigUtils
    {
        template < typename T >
        std::optional< T > findValueByNode(
            std::shared_ptr< ConfigNode > startNode, const std::string& nodeName, const std::string& key );

        std::optional< std::shared_ptr< ConfigNode > > traverseTree(
            std::shared_ptr< ConfigNode > startNode, const std::string nodeName );

        std::optional< std::shared_ptr< ConfigNode > > getConfigNode(
            std::shared_ptr< ConfigNode > startNode, const std::string& nodeName );
    }

    template < typename T >
    std::optional< T > ConfigUtils::findValueByNode(
        std::shared_ptr< ConfigNode > startNode, const std::string& nodeName, const std::string& key )
    {
        std::optional< T > result = std::nullopt;

        std::vector< std::string > configNodeNames;
        Utility::splitString( nodeName, configNodeNames, '.' );

        std::optional< std::shared_ptr< ConfigNode > > traversalNode = nullptr;
        for ( const auto& configNodeName : configNodeNames )
        {
            traversalNode = traverseTree( startNode, configNodeName );
            if ( !traversalNode )
                return result;
        }

        // IF we got here, then we found the node
        // Lookup the Key
        std::map< std::string, Core::PrimitiveVariant > keyValues = traversalNode->get()->getKeyValues();

        auto it = keyValues.find( key );
        if ( it != keyValues.end() )
        {
            if ( auto val = std::get_if< T >( &it->second ) )
                result = *val;
        }

        return result;
    }
}