#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

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

Core::ConfigNode* Core::ConfigurationTree::traverseTree( ConfigNode* node, const std::string nodeName )
{
    if (node == nullptr)
        return nullptr;

    if (node->mName == nodeName)
        return node;

    for (const auto& child : node->mChildren)
    {
        ConfigNode* resultNode = traverseTree( child.get(), nodeName );
        if (resultNode)
            return resultNode;
    }

    return nullptr;
}