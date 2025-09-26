#pragma once

#include <chrono>
#include <optional>
#include <memory>

namespace Utility
{
    std::optional< std::shared_ptr< struct tm > > getCurrentSystemTime();
}