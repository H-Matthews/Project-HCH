#pragma once

#include "core/Configuration/Configurables/ConfigurableFactory.hpp"

#include <string>

namespace Core
{
    class ConfigInitializer
    {
      public:
        static void registerConfigurables();

      private:
        static const std::string CONFIGURABLE_APP;
        static const std::string CONFIGURABLE_STATESTACK;
        static const std::string CONFIGURABLE_NETWORK;
        static const std::string CONFIGURABLE_SCENE;
    };

}