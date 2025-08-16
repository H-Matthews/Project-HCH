#pragma once

#include "core/Configuration/Configurables/ConfigurableFactory.hpp"

#include <string>

namespace Core
{
    class ConfigInitializer
    {
      public:
        static void registerCoreConfigurables();
    };

}