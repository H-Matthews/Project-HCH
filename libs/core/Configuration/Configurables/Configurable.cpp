#include "core/Configuration/Configurables/Configurable.hpp"

#include "core/Configuration/ConfigTree/ConfigUtils.hpp"

Core::Configurable::Configurable( const std::string& typeName ) :
    mConfigNode(),
    mLogger( nullptr ),
    mDirector(),
    mTypeName( typeName )
{
    std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();

    auto configNode = ConfigUtils::getConfigNode( rootNode, mTypeName );
    if ( configNode )
        mConfigNode = *configNode;

    if constexpr ( Utility::CAN_LOG )
    {
        // Check to see if this Configurable has a logger
        bool isLoggingEnabled = false;
        if ( auto* enabled = mConfigNode->findValue< bool >( "logging_enabled" ) )
            isLoggingEnabled = *enabled;

        if ( isLoggingEnabled )
            mLogger = mDirector.buildLogger( mConfigNode );
    }

    return;
}
