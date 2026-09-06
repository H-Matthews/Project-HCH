#include "core/Configuration/ConfigTree/ConfigUtils.hpp"

#include <stack>

namespace Core {
std::optional<std::shared_ptr<ConfigNode>>
ConfigUtils::traverseTree(std::shared_ptr<ConfigNode> startNode, const std::string nodeName) {

    std::optional<std::shared_ptr<ConfigNode>> result = std::nullopt;

    if (startNode == nullptr)
        return result;

    std::stack<std::shared_ptr<ConfigNode>> nodeStack;
    nodeStack.push(startNode);

    while (!nodeStack.empty()) {
        auto currentNode = nodeStack.top();
        nodeStack.pop();

        if (currentNode->mName == nodeName) {
            result = currentNode;

            return result;
        }

        std::vector<std::shared_ptr<Core::ConfigNode>> children = currentNode->getChildren();

        for (const auto& child : children)
            nodeStack.push(child);
    }

    // Didn't find if we get here
    return result;
}

std::optional<std::shared_ptr<ConfigNode>>
ConfigUtils::getConfigNode(std::shared_ptr<ConfigNode> startNode, const std::string& nodeName) {
    std::optional<std::shared_ptr<ConfigNode>> result = std::nullopt;

    result = traverseTree(startNode, nodeName);

    return result;
}
} // namespace Core