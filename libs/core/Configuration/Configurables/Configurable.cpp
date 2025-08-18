#include "core/Configuration/Configurables/Configurable.hpp"

Core::Configurable::Configurable( const std::string& typeName ) :
    mConfigNode(),
    mDirector(),
    mTypeName( typeName )
{
    auto configNode = ConfigurationTree::instance()->getConfigNode( mTypeName );
    if ( configNode )
        mConfigNode = *configNode;

    return;
}

std::shared_ptr< Utility::Logger > Core::Configurable::createLogger()
{
    std::shared_ptr< Utility::Logger > logger = nullptr;

    if ( mConfigNode.expired() )
        return logger;

    // Determine if this configurable is creating a logger
    bool isLoggingEnabled = false;
    if ( auto* enabled = mConfigNode.lock()->findValue< bool >( "logging_enabled" ) )
        isLoggingEnabled = *enabled;

    if ( !isLoggingEnabled )
        return logger;

    logger = mDirector.buildLogger( mConfigNode.lock().get() );

    return logger;
}