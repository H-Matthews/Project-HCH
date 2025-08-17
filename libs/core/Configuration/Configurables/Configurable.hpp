#pragma once

#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"

#include "utility/Logging/Logger.hpp"

#include <string>

namespace Core
{
    /**
     * All objects that are configurable from the configuration files will inherit from this class
     */
    class Configurable
    {
      public:
        Configurable( const std::string& typeName );

        virtual ~Configurable() = default;

      protected:
        std::shared_ptr< Utility::Logger > createLogger();

      protected:
        std::weak_ptr< ConfigNode > mConfigNode;

      private:
        std::string mTypeName;
    };
}