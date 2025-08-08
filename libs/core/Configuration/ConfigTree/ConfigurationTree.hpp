#pragma once

#include <memory>
#include <string>
#include <algorithm>
#include <cstring>

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include "utility/StringOperations.hpp"

namespace Core
{
    class ConfigNode;

    class ConfigurationTree
    {
      public:
        ConfigurationTree( const ConfigurationTree* configTree ) = delete;

        static std::shared_ptr< ConfigurationTree > instance();

        void attachConfigNode( std::shared_ptr< ConfigNode > configNode );

        ConfigNode* traverseTree( ConfigNode* root, const std::string nodeName );

        template < typename T >
        T* findValueByNode( const std::string& nodeName, const std::string& key );

        template < typename T >
        std::vector< T* > findValuesByNode( const std::string& nodeName );

      private:
        ConfigurationTree();

      private:
        // Static pointer to our object
        static std::shared_ptr< ConfigurationTree > mConfigTreeInstance;

        std::shared_ptr< ConfigNode > mRootNode;

      public:
        friend class ConfigNode;
    };

    template < typename T >
    T* ConfigurationTree::findValueByNode( const std::string& nodeName, const std::string& key )
    {
        T* result = nullptr;

        std::vector< std::string > configNodeNames;
        Utility::splitString( nodeName, configNodeNames, '.' );

        ConfigNode* traversalNode = nullptr;
        for ( const auto& configNodeName : configNodeNames )
        {
            traversalNode = traverseTree( mRootNode.get(), configNodeName );
            if ( traversalNode == nullptr )
                return result;
        }

        // IF we got here, then we found the node
        // Lookup the Key
        auto it = traversalNode->mKeyValues.find( key );
        if ( it != traversalNode->mKeyValues.end() )
            result = std::get_if< T >( &it->second );

        return result;
    }

    template < typename T >
    std::vector< T* > ConfigurationTree::findValuesByNode( const std::string& nodeName )
    {
        std::vector< T* > result;

        std::vector< std::string > configNodeNames;
        Utility::splitString( nodeName, configNodeNames, '.' );

        ConfigNode* traversalNode = nullptr;
        for ( const auto& configNodeName : configNodeNames )
        {
            traversalNode = traverseTree( mRootNode.get(), configNodeName );
            if ( traversalNode == nullptr )
                return result;
        }

        // IF we got here, then we found the node with the vector
        for ( auto& value : traversalNode->mArrayValues )
        {
            T* tempPointer = std::get_if< T >( &value );
            if ( tempPointer )
            {
                result.push_back( tempPointer );
            }
        }
    }

}