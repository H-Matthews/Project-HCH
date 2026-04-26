#include "core/Messaging/MessageNode.hpp"
#include "core/Messaging/MessageNetwork.hpp"

Core::MessageNode::MessageNode(
    MessageNetwork& messageNetwork, const std::string& messageNodeName, NodeType nodeType ) :
    mMessageNetwork( messageNetwork ),
    mNodeName( messageNodeName ),
    mCallback( this->getNotifyFunc() ),
    mNodeType( nodeType ),
    mNetworkLogger()
{
    // Get Logger
    if constexpr ( Utility::CAN_LOG )
        mNetworkLogger = Utility::LogRegistry::instance()->getLogger( "MessageNetworkLogger" );

    // Register Node
    switch ( mNodeType )
    {
        case NodeType::PUBLISHER:
        {
            mMessageNetwork.registerPublisherNode( mNodeName );
            break;
        }
        case NodeType::SUBSCRIBER:
        {
            mMessageNetwork.registerSubscriberNode( mNodeName, mCallback );
            break;
        }
        case NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.registerPublisherNode( mNodeName );
            mMessageNetwork.registerSubscriberNode( mNodeName, mCallback );
            break;
        }
    }
}

// ONLY use this function if the node is BOTH a publisher and subscriber
// AND you wish to subscribe to the topic as a subscriber and publisher
// OTHERWISE, use the other functions addSubscriberTopic, addPublisherTopic
void Core::MessageNode::addTopic( Messages::ID messageID )
{
    switch ( mNodeType )
    {
        case Core::NodeType::PUBLISHER:
        {
            mMessageNetwork.addPublisherTopic( mNodeName, messageID );
            break;
        }
        case Core::NodeType::SUBSCRIBER:
        {
            mMessageNetwork.addSubscriberTopic( mNodeName, messageID );
            break;
        }
        case Core::NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.addPublisherTopic( mNodeName, messageID );
            mMessageNetwork.addSubscriberTopic( mNodeName, messageID );
            break;
        }
    }

    return;
}

void Core::MessageNode::addSubscriberTopic( Messages::ID messageID )
{
    mMessageNetwork.addSubscriberTopic( mNodeName, messageID );

    return;
}
void Core::MessageNode::addPublisherTopic( Messages::ID messageID )
{
    mMessageNetwork.addPublisherTopic( mNodeName, messageID );

    return;
}

// ONLY use this function if the node is BOTH a publisher and subscriber
// AND you wish to subscribe to the topic as a subscriber and publisher
// OTHERWISE, use the other functions addSubscriberTopic, addPublisherTopic
void Core::MessageNode::removeTopic( Messages::ID messageID )
{
    switch ( mNodeType )
    {
        case Core::NodeType::PUBLISHER:
        {
            mMessageNetwork.addPublisherTopic( mNodeName, messageID );
            break;
        }
        case Core::NodeType::SUBSCRIBER:
        {
            mMessageNetwork.addSubscriberTopic( mNodeName, messageID );
            break;
        }
        case Core::NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.addPublisherTopic( mNodeName, messageID );
            mMessageNetwork.addSubscriberTopic( mNodeName, messageID );
            break;
        }
    }
}

void Core::MessageNode::removeSubscriberTopic( Messages::ID messageID )
{
    mMessageNetwork.removeTopicFromSubscriber( mNodeName, messageID );

    return;
}
void Core::MessageNode::removePublisherTopic( Messages::ID messageID )
{
    mMessageNetwork.removeTopicFromPublisher( mNodeName, messageID );

    return;
}

void Core::MessageNode::unRegisterNode()
{
    switch ( mNodeType )
    {
        case Core::NodeType::PUBLISHER:
        {
            mMessageNetwork.unRegisterPublisherNode( mNodeName );
            break;
        }
        case Core::NodeType::SUBSCRIBER:
        {
            mMessageNetwork.unRegisterSubscriberNode( mNodeName );
            break;
        }
        case Core::NodeType::SUB_AND_PUB:
        {
            mMessageNetwork.unRegisterPublisherNode( mNodeName );
            mMessageNetwork.unRegisterSubscriberNode( mNodeName );
            break;
        }
    }
    return;
}
void Core::MessageNode::unRegisterSubscriberNode()
{
    mMessageNetwork.unRegisterSubscriberNode( mNodeName );

    return;
}
void Core::MessageNode::unRegisterpublisherNode()
{
    mMessageNetwork.unRegisterPublisherNode( mNodeName );

    return;
}

void Core::MessageNode::publish( std::shared_ptr< Message > message )
{
    message->setSender( mNodeName );

    if ( message->getMessageID() != Messages::ID::NONE )
    {
        mMessageNetwork.publishMessage( message );
    }
    else
    {
        if constexpr ( Utility::CAN_LOG_ERROR )
            mNetworkLogger->logError( "Could not Publisher message from Node " + message->getSenderName() +
                                      " due to no topic being associated with Message" );
    }
}

void Core::MessageNode::onNotify( Message* )
{
    if constexpr ( Utility::CAN_LOG_ERROR )
        mNetworkLogger->logError( "onNotify(Message) IS NOT implemented for Node " + mNodeName );
}

std::function< void( Core::Message* ) > Core::MessageNode::getNotifyFunc()
{
    auto messageSubscriber = [ =, this ]( Message* message ) -> void { this->onNotify( message ); };

    return messageSubscriber;
}