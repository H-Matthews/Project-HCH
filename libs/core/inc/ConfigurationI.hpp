#pragma once

// Defines our Configuration Interface
namespace Core
{
    class ConfigurationI
    {
        public:
            virtual void initializeIteration() = 0;

            virtual void loadSettings() = 0;
    };
    
}