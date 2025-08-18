#pragma once

#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace Core
{
    using PrimitiveVariant = std::variant< bool, char, int, float, double, std::string >;

    class ConfigNode
    {
      public:
        ConfigNode( const std::string& name );
        ConfigNode( const std::string& name, std::shared_ptr< ConfigNode > parent );

        void addChild( std::shared_ptr< ConfigNode > childNode );
        ConfigNode* getChild( const std::string& name );

        void setParent( std::shared_ptr< ConfigNode > parentNode );
        void insertValuePair( const std::string& key, const PrimitiveVariant& value );
        void insertArrayValue( const PrimitiveVariant& value );

        // TODO: These functions essentially exist in the ConfigurationTree
        // Need to see if we can consolidate these functions
        template < typename T >
        T* findValue( const std::string& key );

        // TODO: This really needs to go into some utility class for ConfigNodes
        static ConfigNode* findRelativeNode( const std::string& nodeName, ConfigNode* node );

      private:
        std::string mName;

        std::shared_ptr< ConfigNode > mParent;
        std::vector< std::shared_ptr< ConfigNode > > mChildren;

        std::map< std::string, PrimitiveVariant > mKeyValues;
        std::vector< PrimitiveVariant > mArrayValues;

      public:
        friend class ConfigurationTree;
    };

    template < typename T >
    T* Core::ConfigNode::findValue( const std::string& key )
    {
        T* primitiveType = nullptr;

        auto it = mKeyValues.find( key );
        if ( it != mKeyValues.end() )
            primitiveType = std::get_if< T >( &it->second );

        return primitiveType;
    }
}