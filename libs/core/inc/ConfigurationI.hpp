#pragma once

#include <string>

// Defines our Configuration Interface
namespace Core
{
    class ConfigurationI
    {
        public:
            virtual void initializeIteration() = 0;

            virtual void parseConfigs() = 0;

            virtual ~ConfigurationI() {}
    };
    
}