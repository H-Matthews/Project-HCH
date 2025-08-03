#pragma once

#include <string>

namespace Core
{
    struct ConfigType
    {
        ConfigType( const std::string& type ) :
            configType( type )
        {}

        std::string configType;

        virtual ~ConfigType()
        {}
    };
}