#pragma once

#include <memory>

// Forward Declaration
namespace Utility {
class Logger;
}

namespace Core {
class ConfigNode;

class ConfigurableDirector {
  public:
    std::shared_ptr<Utility::Logger> buildLogger(std::shared_ptr<ConfigNode> node);
};
} // namespace Core