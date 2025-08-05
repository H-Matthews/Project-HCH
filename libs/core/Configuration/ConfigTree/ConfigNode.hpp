#pragma once

#include <string>
#include <memory>
#include <variant>
#include <vector>
#include <map>

namespace Core
{
    using PrimitiveVariant = std::variant< char, int, float, double, std::string >;

    class ConfigNode
    {
      public:
        ConfigNode( const std::string& name );
        ConfigNode( const std::string& name, std::shared_ptr< ConfigNode > parent );

        void addChild( std::shared_ptr< ConfigNode > childNode );
        void setParent( std::shared_ptr< ConfigNode > parentNode );
        void insertValuePair( const std::string& key, const PrimitiveVariant& value );
        void insertArrayValue( const PrimitiveVariant& value );

        template < typename T >
        T* find( const std::string& key );

      private:
        std::string mName;

        std::shared_ptr< ConfigNode > mParent;
        std::vector< std::shared_ptr< ConfigNode > > mChildren;

        std::multimap< std::string, PrimitiveVariant > mKeyValues;
        std::vector< PrimitiveVariant > mArrayValues;
    };

    template < typename T >
    T* Core::ConfigNode::find( const std::string& key )
    {
        T* primitiveType = nullptr;

        auto it = mKeyValues.find( key );
        if ( it != mKeyValues.end() )
            primitiveType = std::get_if< T >( it->second );

        return primitiveType;
    }
}