#pragma once

#include "core/Configuration/ConfigTree/ConfigurationTree.hpp"
#include "core/Configuration/Configurables/ConfigurableDirector.hpp"

#include "utility/Logging/Logger.hpp"

#include <string>

namespace Core
{
    /**
     * Defines an object in the application that is Configurable
     * through configuration files
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