#include "utility\Time.hpp"

#include <iostream>
#include <optional>

namespace Utility
{
    std::optional< std::shared_ptr< struct tm > > getCurrentSystemTime()
    {
        std::optional< std::shared_ptr< struct tm > > optionalTime = std::nullopt;

        std::time_t rawTime;
        time( &rawTime );

        auto testTime = std::make_shared< struct tm >();
        errno_t testErr = localtime_s( testTime.get(), &rawTime );
        if (!testErr)
            optionalTime = testTime;

        return optionalTime;
    }
}