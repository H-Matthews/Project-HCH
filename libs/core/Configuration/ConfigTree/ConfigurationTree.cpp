#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <stack>

std::shared_ptr<Core::ConfigurationTree> Core::ConfigurationTree::mConfigTreeInstance = nullptr;

Core::ConfigurationTree::ConfigurationTree()
    : mRootNode(std::make_unique<ConfigNode>("Project-HCH")) {}

std::shared_ptr<Core::ConfigurationTree> Core::ConfigurationTree::instance() {
    if (mConfigTreeInstance == nullptr)
        mConfigTreeInstance =
            std::shared_ptr<Core::ConfigurationTree>(new Core::ConfigurationTree());

    return mConfigTreeInstance;
}

void Core::ConfigurationTree::attachConfigNode(std::shared_ptr<ConfigNode> configNode) {
    configNode->setParent(mRootNode);
    mRootNode->addChild(configNode);

    return;
}

std::shared_ptr<Core::ConfigNode> Core::ConfigurationTree::getRootNode() {
    return mRootNode;
}