#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

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

      private:
        ConfigurationTree();

      private:
        // Static pointer to our object
        static std::shared_ptr< ConfigurationTree > mConfigTreeInstance;

        std::shared_ptr< ConfigNode > mRootNode;

      public:
        friend class ConfigNode;
    };

    // root.configuration_files

    template < typename T >
    T* ConfigurationTree::findValueByNode( const std::string& nodeName, const std::string& key )
    {
        T* result = nullptr;

        auto pos = nodeName.find( '.' );

        std::string temp = nodeName;
        std::vector< std::string > configNodeNames;

        while (pos != std::string::npos)
        {
            // Extract the substring
            std::string configName = temp.substr( 0, pos );
            configNodeNames.push_back( configName );

            // Erase extracted part
            temp.erase( 0, pos + 1 );

            // Find next occurrence of delimiter
            pos = temp.find( '.' );
        }

        configNodeNames.push_back( temp );

        ConfigNode* traversalNode = nullptr;
        for (const auto& configNodeName : configNodeNames)
        {
            traversalNode = traverseTree( mRootNode.get(), configNodeName );
            if (traversalNode == nullptr)
                return result;
        }

        // IF we got here, then we found the node
        // Lookup the Key
        auto it = traversalNode->mKeyValues.find( key );
        if (it != traversalNode->mKeyValues.end())
            result = std::get_if< T >( &it->second );

        return result;
    }

}