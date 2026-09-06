#pragma once

#include <memory>
#include <string>
#include <algorithm>
#include <cstring>
#include <optional>

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

#include "utility/StringOperations.hpp"

namespace Core {
class ConfigNode;

/**
 * Singleton class that holds the root config node
 *
 * Mainly used so that each Configurable can grab its Config Node
 */
class ConfigurationTree {
  public:
    ConfigurationTree(const ConfigurationTree* configTree) = delete;

    static std::shared_ptr<ConfigurationTree> instance();

    void attachConfigNode(std::shared_ptr<ConfigNode> configNode);

    std::shared_ptr<ConfigNode> getRootNode();

  public:
    friend class ConfigNode;

  private:
    ConfigurationTree();

  private:
    // Static pointer to our object
    static std::shared_ptr<ConfigurationTree> mConfigTreeInstance;

    std::shared_ptr<ConfigNode> mRootNode;
};

} // namespace Core