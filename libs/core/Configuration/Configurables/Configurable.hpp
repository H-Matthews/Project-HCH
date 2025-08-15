#pragma once

#include "core/Configuration/ConfigTree/ConfigNode.hpp"

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

      private:
        std::string mTypeName;

        std::shared_ptr< ConfigNode > mConfigNode;
    };
}