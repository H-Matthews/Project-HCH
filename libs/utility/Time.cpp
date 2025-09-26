#include "utility/Time.hpp"

#include <iostream>
#include <optional>
#include <cerrno>

namespace Utility
{
    std::tm getCurrentSystemTime()
    {
        // Get Time in a broken down structure
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t( now );
        std::tm now_tm = *std::localtime( &nowTime );

        return now_tm;
    }
}