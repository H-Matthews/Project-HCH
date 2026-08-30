#include "core/Configuration/Configurables/Configurable.hpp"

#include "core/Configuration/ConfigTree/ConfigUtils.hpp"

Core::Configurable::Configurable( const std::string& typeName ) :
    mConfigNode( nullptr ),
    mLogger( nullptr ),
    mDirector(),
    mTypeName( typeName )
{
    // If either node is hasn't been created yet, just return
    std::shared_ptr< ConfigNode > rootNode = ConfigurationTree::instance()->getRootNode();
    std::vector< std::shared_ptr< Core::ConfigNode > > childrenNodes = rootNode->getChildren();
    if (childrenNodes.empty())
        return;

    this->configure( rootNode );

    return;
}

void Core::Configurable::configure( std::shared_ptr< ConfigNode > rootNode )
{
    auto configNode = ConfigUtils::getConfigNode( rootNode, mTypeName );
    if (configNode)
        mConfigNode = *configNode;

    // CONFIGURE LOGGER ---------------------------
    if constexpr (Utility::CAN_LOG)
    {
        // Check to see if this Configurable has a logger
        bool isLoggingEnabled = false;
        if (const auto* enabled = mConfigNode->findValue< bool >( "logging_enabled" ))
            isLoggingEnabled = *enabled;

        if (isLoggingEnabled)
            mLogger = mDirector.buildLogger( mConfigNode );
    }

    return;
}
