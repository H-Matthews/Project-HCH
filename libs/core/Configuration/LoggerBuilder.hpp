#pragma once

#include "core/Configuration/ConfigSection/ConfigSection.hpp"
#include "utility/Logging/Logger.hpp"

#include <memory>

namespace Core {
/**
 * Reads logger configuration from a subsystem's ConfigSection and constructs
 * the Logger with its sinks and formatters. Returns nullptr if logging_enabled
 * is false or the Logger sub-section is absent. Registers the built logger
 * with LogRegistry so it can be retrieved by name.
 */
std::shared_ptr<Utility::Logger> buildLogger(const ConfigSection& config);
} // namespace Core
