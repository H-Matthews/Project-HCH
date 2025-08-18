#pragma once

#include <memory>

// Forward Declaration
namespace Utility
{
    class Logger;
}

namespace Core
{
    class ConfigNode;

    class ConfigurableDirector
    {
      public:
        std::shared_ptr< Utility::Logger > buildLogger( ConfigNode* node );
    };
}