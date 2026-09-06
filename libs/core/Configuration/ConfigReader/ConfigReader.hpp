#pragma once

#include <filesystem>
#include <string>

namespace Core {
class ConfigNode;

/**
 * Interface class for config readers
 * Config Reader implementations should return a ConfigNode that contains the parsed data
 *
 * Primarily used in the Configuration class
 */
class ConfigReader {
  public:
    virtual void init() {}

    virtual std::pair<bool, std::string> readFile(const std::filesystem::path& filePath,
                                                  std::shared_ptr<ConfigNode>& configNode) = 0;
};
} // namespace Core