#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Core {
/**
 * Format-agnostic interface representing one table of the parsed configuration.
 * Subsystems receive a ConfigSection scoped to their own block (e.g. [StateStack])
 * and navigate to nested tables via getSection(), keeping them decoupled from any
 * specific config file format or parser library.
 */
class ConfigSection {
  public:
    virtual ~ConfigSection() = default;

    virtual std::optional<bool> getBool(std::string_view key) const = 0;
    virtual std::optional<int> getInt(std::string_view key) const = 0;
    virtual std::optional<double> getDouble(std::string_view key) const = 0;
    virtual std::optional<std::string> getString(std::string_view key) const = 0;
    virtual std::vector<std::string> getStringVector(std::string_view key) const = 0;

    virtual std::unique_ptr<ConfigSection> getSection(std::string_view name) const = 0;
    virtual std::vector<std::string> sectionNames() const = 0;
};

} // namespace Core
