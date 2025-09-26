#pragma once

#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"
#include "core/Configuration/Configurables/ConfigurableDirector.hpp"

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
        void configure( std::shared_ptr< ConfigNode > rootNode );

      protected:
        std::shared_ptr< ConfigNode > mConfigNode;

        std::shared_ptr< Utility::Logger > mLogger;

      private:
        ConfigurableDirector mDirector;

        std::string mTypeName;
    };
}