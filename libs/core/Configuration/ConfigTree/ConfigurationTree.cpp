#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <stack>

std::shared_ptr< Core::ConfigurationTree > Core::ConfigurationTree::mConfigTreeInstance = nullptr;

Core::ConfigurationTree::ConfigurationTree() :
    mRootNode( std::make_unique< ConfigNode >( "Project-HCH" ) )
{}

std::shared_ptr< Core::ConfigurationTree > Core::ConfigurationTree::instance()
{
    if (mConfigTreeInstance == nullptr)
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
Core::ConfigNode* Core::ConfigurationTree::traverseTree( ConfigNode* node, const std::string nodeName )
{
    if (node == nullptr)
        return nullptr;

    std::stack< ConfigNode* > nodeStack;
    nodeStack.push( node );

    while (!nodeStack.empty())
    {
        ConfigNode* currentNode = nodeStack.top();
        nodeStack.pop();

        if (currentNode->mName == nodeName)
            return currentNode;

        for (const auto& child : currentNode->mChildren)
            nodeStack.push( child.get() );
    }

    return nullptr;
}