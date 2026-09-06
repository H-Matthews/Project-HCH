#pragma once

#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace Core {
using PrimitiveVariant =
    std::variant<bool, char, int, float, double, std::string, std::vector<bool>,
                 std::vector<int64_t>, std::vector<double>, std::vector<std::string>>;

class ConfigNode {
  public:
    ConfigNode(const std::string& name);
    ConfigNode(const std::string& name, std::shared_ptr<ConfigNode> parent);

    void addChild(std::shared_ptr<ConfigNode> childNode);
    ConfigNode* getChild(const std::string& name);

    std::vector<std::shared_ptr<ConfigNode>> getChildren();

    void setParent(std::shared_ptr<ConfigNode> parentNode);
    void insertValuePair(const std::string& key, const PrimitiveVariant& value);

    std::map<std::string, PrimitiveVariant> getKeyValues();

    template <typename T> T* findValue(const std::string& key);

  public:
    std::string mName;

  private:
    std::shared_ptr<ConfigNode> mParent;
    std::vector<std::shared_ptr<ConfigNode>> mChildren;

    std::map<std::string, PrimitiveVariant> mKeyValues;

  public:
    friend class ConfigurationTree;
};

template <typename T> T* Core::ConfigNode::findValue(const std::string& key) {
    T* primitiveType = nullptr;

    auto it = mKeyValues.find(key);
    if (it != mKeyValues.end())
        primitiveType = std::get_if<T>(&it->second);

    return primitiveType;
}
} // namespace Core