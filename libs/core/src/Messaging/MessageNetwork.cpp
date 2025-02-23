#include "core/inc/Messaging/MessageNetwork.hpp"

#include "utility/inc/Logging/Sinks/TextFileSink.hpp"
#include "utility/inc/Logging/Formatters/KeyValueFormatter.hpp"

#include <iostream>

Core::MessageNetwork::MessageNetwork() :
    mMessageQueue(),
    mSubscriberNodes(),
    mPublisherNodes(),
    mSubscriberCallBacks(),
    mPendingPublisherRequests(),
    mPendingSubscriberRequests(),
    mHash(),
    mLogger(nullptr)
{
}

void Core::MessageNetwork::publishMessage(std::shared_ptr<Message> message)
{
    if constexpr (Utility::CAN_LOG)
    {
        std::string logMessage("Adding Message " + message->getStringMessageID() + " from " + 
            message->getSenderName() + " to the network");

        mLogger->logDebug(logMessage);
    }

    // Add to Queue
    mMessageQueue.push(message);
}

void Core::MessageNetwork::addPublisherTopic(const std::string& nodeName, Messages::ID messageID )
{
    std::size_t hashValue = mHash(nodeName);

    auto it = mPublisherNodes.find(hashValue);
    if( it == mPublisherNodes.end())
    {
        mLogger->logWarn("Node: " + nodeName + " is NOT registered as a Publisher");
    }
    else
    {
        auto retPair = it->second.insert(messageID);
        if(!retPair.second)
            mLogger->logWarn("Node: " + nodeName + " is already a publisher for topic " + messageIDEnumToString(messageID));
    }
    
    return;
}

void Core::MessageNetwork::addSubscriberTopic(const std::string& nodeName, Messages::ID messageID)
{
    std::size_t hashValue = mHash(nodeName);

    auto it = mSubscriberNodes.find(hashValue);
    if( it == mSubscriberNodes.end())
    {
        mLogger->logError("Node: " + nodeName + " is NOT registered as a Subscriber");
    }
    else
    {
        auto retPair = it->second.insert(messageID);
        if(!retPair.second)
            mLogger->logError("Node: " + nodeName + " is already subscriber to topic " + messageIDEnumToString(messageID));
    }

    return;
}

void Core::MessageNetwork::registerSubscriberNode(const std::string& nodeName, std::function<void (Message*) > callback)
{
    std::size_t hashValue = mHash(nodeName);

    // IF hashValue is not registered, register node
    auto it = mSubscriberNodes.find(hashValue);
    if(it == mSubscriberNodes.end())
    {
        std::set< Messages::ID > set;
        mSubscriberNodes.insert( {hashValue, set} );

        // Register callback for subscriber in map
        mSubscriberCallBacks.insert( {hashValue, callback});

        if constexpr(Utility::CAN_LOG)
            mLogger->logInfo("Registering Subscriber Node -- " + nodeName);
    }
    else
    {
        mLogger->logError("Node: " + nodeName + " is already registered as a subscriber ");
    }

}

void Core::MessageNetwork::registerPublisherNode(const std::string& nodeName)
{
    std::size_t hashValue = mHash(nodeName);

    auto it = mPublisherNodes.find(hashValue);
    if(it == mPublisherNodes.end())
    {
        std::set< Messages::ID > set;
        mPublisherNodes.insert( {hashValue, set} );

        if constexpr(Utility::CAN_LOG)
            mLogger->logInfo("Registering Pubilsher Node -- " + nodeName);
    }
    else
    {
        mLogger->logError("Node: " + nodeName + " is already registered as a publisher " );
    }
}

void Core::MessageNetwork::removeTopicFromPublisher(const std::string& nodeName, Messages::ID messageID)
{
    std::size_t hashValue = mHash(nodeName);

    auto it = mPublisherNodes.find(hashValue);
    if(it == mPublisherNodes.end())
    {
        mLogger->logError("Node: " + nodeName + " was NOT registered as a publisher " );
    }
    else
    {
        auto it = mPendingPublisherRequests.find(hashValue);
        if(it == mPendingPublisherRequests.end())
        {
            std::set<Messages::ID> set = { messageID };
            mPendingPublisherRequests.insert( {hashValue, set} );
        }
        else
        {
            it->second.insert(messageID);
        }
    }

    return;
}

void Core::MessageNetwork::removeTopicFromSubscriber(const std::string& nodeName, Messages::ID messageID)
{
    std::size_t hashValue = mHash(nodeName);

    auto it = mSubscriberNodes.find(hashValue);
    if(it == mSubscriberNodes.end())
    {
        mLogger->logError("Node: " + nodeName + " was NOT registered as a publisher " );
    }
    else
    {
        auto it = mPendingSubscriberRequests.find(hashValue);
        if(it == mPendingSubscriberRequests.end())
        {
            std::set<Messages::ID> set = { messageID };
            mPendingSubscriberRequests.insert( {hashValue, set} );
        }
        else
        {
            it->second.insert(messageID);
        }
    }


    return;
}

void Core::MessageNetwork::notifySubscribers()
{
    std::string logMessage;
    Messages::ID messageID;

    while( !mMessageQueue.empty())
    {
        messageID = mMessageQueue.front().get()->getMessageID();

        if constexpr (Utility::CAN_LOG)
            logMessage += "Sending Message " + messageIDEnumToString(messageID) + " published by " +
                mMessageQueue.front()->getSenderName() + " to subscribers";

        for(const auto& [hashValue, messageSet] : mSubscriberNodes)
        {
            // Lookup messageID in subscribers SET
            auto it = messageSet.find(messageID);
            if(it != messageSet.end())
            {
                // IF we find the messageID in subscribers set, then lookup the subscribers callback
                auto it = mSubscriberCallBacks.find(hashValue);
                if(it != mSubscriberCallBacks.end())
                    it->second(mMessageQueue.front().get());
            }
        }

        if constexpr (Utility::CAN_LOG)
        {
            mLogger->logDebug(logMessage);
            logMessage.clear();
        }

        mMessageQueue.pop();
    }  

    // Remove Topics, if needed
    if(!mPendingPublisherRequests.empty() || !mPendingSubscriberRequests.empty())
        addressPendingRequests();
}

void Core::MessageNetwork::addressPendingRequests()
{

    // Fulfill Publisher Requests
    for (const auto& [hashValue, messageSet] : mPendingPublisherRequests)
    {
        auto publisherNodesIT = mPublisherNodes.find(hashValue);
        if(publisherNodesIT == mPublisherNodes.end())
            continue;

        for(auto messageSetIT = messageSet.begin(); messageSetIT != messageSet.end(); messageSetIT++)
        {
            auto messageIT = publisherNodesIT->second.find( *(messageSetIT) );
            if(messageIT != messageSet.end())
                publisherNodesIT->second.erase(messageIT);
        }
    }

    mPendingPublisherRequests.clear();

    // Fulfill Subscriber Requests
    for (const auto& [hashValue, messageSet] : mPendingSubscriberRequests)
    {
        auto subscriberNodesIT = mSubscriberNodes.find(hashValue);
        if(subscriberNodesIT == mSubscriberNodes.end())
            continue;

        for(auto messageSetIT = messageSet.begin(); messageSetIT != messageSet.end(); messageSetIT++)
        {
            auto messageIT = subscriberNodesIT->second.find( *(messageSetIT) );
            if(messageIT != messageSet.end())
                subscriberNodesIT->second.erase(messageIT);
        }
    }

    mPendingSubscriberRequests.clear();

    return;
}

void Core::MessageNetwork::initializeLogger()
{
    // Create and Register
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();

    mLogger = Utility::createTextFileLogger("MessageNetworkLogger", outDirectory, "MessageNetwork", 
                                                     ".log", Utility::LogLevel::DEBUG);
                                                     
    if constexpr (Utility::CAN_LOG)
        mLogger->logInfo("Logger Initialized");
}

void Core::MessageNetwork::shutdownNetwork()
{
    mSubscriberNodes.clear();
    mPublisherNodes.clear();

    // This "clears" the Queue
    mMessageQueue = {};
}