#pragma once

#include <string>
#include <memory>

#include "core/Configuration/ConfigFileID.hpp"
#include "core/Configuration/ConfigReader/ConfigType/ConfigType.hpp"

namespace Core
{
    /**
     * A ConfigNode is a defined structure of data that is created from a ConfigReader
     */
    struct ConfigNode
    {
        ConfigNode() :
            configFileID( ConfigFileID::SIZE ),
            retStatus(),
            configType( nullptr )
        {}

        ConfigFileID configFileID;
        std::pair< bool, std::string > retStatus;

        // Contains the parsed data
        std::shared_ptr< ConfigType > configType;

        template < typename T >
        std::shared_ptr< T > getTypedConfig();
    };

    template < typename T >
    std::shared_ptr< T > ConfigNode::getTypedConfig()
    {
        // std::shared_ptr< T > typedConfig = std::dynamic_pointer_cast< T >( configType );

        return std::dynamic_pointer_cast< T >( configType );
    }
}