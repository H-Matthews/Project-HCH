#pragma once

// Defines our Configuration Interface
namespace Core
{
    class ConfigurationI
    {
        public:
            virtual bool initializeIteration() = 0;

            virtual void loadSettings() = 0;

            virtual ~ConfigurationI() {}
    };
    
}