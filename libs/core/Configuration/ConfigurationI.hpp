#pragma once

#include <string>

namespace Core
{

    /**
     * Defines our Configuration Interface
     */

    class ConfigurationI
    {
      public:
        virtual void initializeIteration() = 0;

        virtual void parseConfigs() = 0;

        virtual ~ConfigurationI()
        {}
    };

}