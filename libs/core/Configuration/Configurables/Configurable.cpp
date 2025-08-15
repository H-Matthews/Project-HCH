#include "core/Configuration/Configurables/Configurable.hpp"

Core::Configurable::Configurable( const std::string& typeName ) :
    mTypeName( typeName ),
    mConfigNode( nullptr )
{}