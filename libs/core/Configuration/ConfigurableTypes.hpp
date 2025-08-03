#pragma once

#include "core/Configuration/ConfigurableHelperTypes.hpp"

namespace Core
{
    struct CoreConfigurable
    {
        std::string configurableType;
        bool loggingEnabled;

        LoggerConfigurable logger;
    };

    // FUTURE
    struct PrefabConfigurable
    {
        int x;
    };

}