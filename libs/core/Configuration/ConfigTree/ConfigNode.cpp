#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <stack>

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

Core::ConfigNode* Core::ConfigNode::findRelativeNode( const std::string& nodeName, ConfigNode* node )
{
    if ( node == nullptr )
        return nullptr;

    std::stack< ConfigNode* > nodeStack;
    nodeStack.push( node );

    while ( !nodeStack.empty() )
    {
        auto currentNode = nodeStack.top();
        nodeStack.pop();

        if ( currentNode->mName == nodeName )
            return currentNode;

        for ( const auto& child : currentNode->mChildren )
            nodeStack.push( child.get() );
    }

    return nullptr;
}

Core::ConfigNode* Core::ConfigNode::getChild( const std::string& name )
{
    for ( const auto& child : mChildren )
    {
        if ( child->mName == name )
            return child.get();
    }

    return nullptr;
}