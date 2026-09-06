#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include <stack>

Core::ConfigNode::ConfigNode(const std::string& name)
    : mName(name), mParent(nullptr), mChildren(), mKeyValues() {}

Core::ConfigNode::ConfigNode(const std::string& name, std::shared_ptr<ConfigNode> parent)
    : mName(name), mParent(parent), mChildren(), mKeyValues() {}

void Core::ConfigNode::addChild(std::shared_ptr<ConfigNode> childNode) {
    if (!childNode)
        return;

    mChildren.push_back(childNode);

    return;
}

/**
 * Caller DOES NOT own these, DONT DELETE
 */
std::vector<std::shared_ptr<Core::ConfigNode>> Core::ConfigNode::getChildren() {
    return mChildren;
}

void Core::ConfigNode::setParent(std::shared_ptr<ConfigNode> parentNode) {
    this->mParent = parentNode;

    return;
}

void Core::ConfigNode::insertValuePair(const std::string& key, const PrimitiveVariant& value) {
    auto it = mKeyValues.find(key);
    if (it == mKeyValues.end())
        mKeyValues[key] = value;

    return;
}

std::map<std::string, Core::PrimitiveVariant> Core::ConfigNode::getKeyValues() {
    return mKeyValues;
}

Core::ConfigNode* Core::ConfigNode::getChild(const std::string& name) {
    for (const auto& child : mChildren) {
        if (child->mName == name)
            return child.get();
    }

    return nullptr;
}