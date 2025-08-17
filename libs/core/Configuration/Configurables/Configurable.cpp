#include "core/Configuration/Configurables/Configurable.hpp"

#include "utility/Logging/Sinks/ColorConsoleSink.hpp"

Core::Configurable::Configurable( const std::string& typeName ) :
    mConfigNode(),
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
    if ( auto* enabled = mConfigNode.lock()->find< bool >( "logging_enabled" ) )
        isLoggingEnabled = *enabled;

    if ( !isLoggingEnabled )
        return logger;

    // Need to build Logger object based on the ConfigSettings
    Utility::Logger* retLogger =
        Utility::Logger::build().name( "ConfigurableTest" ).logLevel( "NONE" ).sinks( { "Console " } );

    Utility::ColorConsoleSink* sink = Utility::ColorConsoleSink::build().logLevel( "DEBUG" );

    logger.reset( retLogger );

    return logger;
}