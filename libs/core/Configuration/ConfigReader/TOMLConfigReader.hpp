#pragma once

#include "core/Configuration/ConfigReader/ConfigReader.hpp"

#include "vendor/toml/include/toml.hpp"

namespace Core {

/**
 * ConfigReader that parses TOML files
 * Returns a config node that contains the parsed data from the toml file
 *
 * NOTE: Utilizes the tomlplusplus library
 */
class TOMLConfigReader : public ConfigReader {
  public:
    std::pair<bool, std::string> readFile(const std::filesystem::path& filePath,
                                          std::shared_ptr<ConfigNode>& configNode) override;

  private:
    void processTOMLData(const toml::node& tomlNode, std::shared_ptr<ConfigNode> configNode);

    void processPrimitiveTOMLData(const std::string& keyNode, const toml::node& tomlNode,
                                  std::shared_ptr<ConfigNode> configNode);

    void processArrayTOMLData(const std::string& keyNode, const toml::node& tomlNode,
                              std::shared_ptr<ConfigNode> configNode);

  private:
    static const std::string PARAM_FILE_TYPE;
};

} // namespace Core