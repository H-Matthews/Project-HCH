#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"
#include "utility/Logging/Logger.hpp"

#include <memory>

namespace Core
{
    std::shared_ptr<Utility::Logger> buildLogger(const ConfigSection& config);
}
