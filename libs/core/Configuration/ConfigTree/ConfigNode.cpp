#include "core/Configuration/ConfigTree/ConfigNode.hpp"

Core::ConfigNode::ConfigNode( const std::string& name ) :
    mName( name ),
    mParent( nullptr ),
    mChildren(),
    mKeyValues(),
    mArrayValues()
{}

Core::ConfigNode::ConfigNode( const std::string& name, std::shared_ptr< ConfigNode > parent ) :
    mName( name ),
    mParent( parent ),
    mChildren(),
    mKeyValues(),
    mArrayValues()
{}

void Core::ConfigNode::addChild( std::shared_ptr< ConfigNode > childNode )
{
    if ( !childNode )
        return;

    mChildren.push_back( childNode );

    return;
}

void Core::ConfigNode::setParent( std::shared_ptr< ConfigNode > parentNode )
{
    this->mParent = parentNode;

    return;
}

void Core::ConfigNode::insertValuePair( const std::string& key, const PrimitiveVariant& value )
{
    mKeyValues[ key ] = value;

    return;
}

void Core::ConfigNode::insertArrayValue( const PrimitiveVariant& value )
{
    mArrayValues.push_back( value );

    return;
}