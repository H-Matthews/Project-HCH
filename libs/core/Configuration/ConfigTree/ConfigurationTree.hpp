#pragma once

#include <memory>

namespace Core
{
    class ConfigNode;

    class ConfigurationTree
    {
      public:
        ConfigurationTree( const ConfigurationTree* configTree ) = delete;

        static std::shared_ptr< ConfigurationTree > instance();

        void addConfigNode( std::shared_ptr< ConfigNode > configNode );

        template < typename T >
        T* find( const std::string& path );

      private:
        ConfigurationTree();

      private:
        // Static pointer to our object
        static std::shared_ptr< ConfigurationTree > mConfigTreeInstance;

        std::shared_ptr< ConfigNode > mRootNode;
    };

    // root.configuration_files

    template < typename T >
    T* Core::ConfigurationTree::find( const std::string& path )
    {

        // Root has the list of files
        auto it = mRootNode->find< T >( fileName );
    }

}