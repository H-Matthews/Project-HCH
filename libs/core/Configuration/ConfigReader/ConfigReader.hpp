#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"

#include <filesystem>
#include <memory>

namespace Core {
class ConfigReader {
  public:
    virtual ~ConfigReader() = default;
    virtual std::unique_ptr<ConfigSection> readFile(const std::filesystem::path& filePath) = 0;
};
} // namespace Core
