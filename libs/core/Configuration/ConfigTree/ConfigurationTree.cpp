#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <stack>

std::shared_ptr< Core::ConfigurationTree > Core::ConfigurationTree::mConfigTreeInstance = nullptr;

Core::ConfigurationTree::ConfigurationTree() :
    mRootNode( std::make_unique< ConfigNode >( "Project-HCH" ) )
{}

std::shared_ptr< Core::ConfigurationTree > Core::ConfigurationTree::instance()
{
    if ( mConfigTreeInstance == nullptr )
        mConfigTreeInstance = std::shared_ptr< Core::ConfigurationTree >( new Core::ConfigurationTree() );

    return mConfigTreeInstance;
}

void Core::ConfigurationTree::attachConfigNode( std::shared_ptr< ConfigNode > configNode )
{
    configNode->setParent( mRootNode );
    mRootNode->addChild( configNode );

    return;
}

/**
 * Itertaively traversing the tree, instead of using recursion
 */
std::shared_ptr< Core::ConfigNode > Core::ConfigurationTree::traverseTree( const std::string nodeName )
{
    if ( mRootNode == nullptr )
        return nullptr;

    std::stack< std::shared_ptr< ConfigNode > > nodeStack;
    nodeStack.push( mRootNode );

    while ( !nodeStack.empty() )
    {
        auto currentNode = nodeStack.top();
        nodeStack.pop();

        if ( currentNode->mName == nodeName )
            return currentNode;

        for ( const auto& child : currentNode->mChildren )
            nodeStack.push( child );
    }

    return nullptr;
}

std::optional< std::shared_ptr< Core::ConfigNode > > Core::ConfigurationTree::getConfigNode(
    const std::string& nodeName )
{
    std::optional< std::shared_ptr< ConfigNode > > result = std::nullopt;

    result = traverseTree( nodeName );

    return result;
}