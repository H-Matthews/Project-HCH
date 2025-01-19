#include "core/inc/Messaging/MessageNetwork.hpp"

#include "utility/inc/Logging/Sinks/TextFileSink.hpp"

#include <iostream>

Core::MessageNetwork::MessageNetwork() :
    mLogger(nullptr),
    mSubscriberList(),
    mMessageQueue()
{
}

void Core::MessageNetwork::sendMessage(std::shared_ptr<Message> message)
{
    if constexpr (Utility::CAN_LOG)
    {
        std::string logMessage("[Sending Message] Sender: " + message->getSenderName() + 
                            " MessageID: " + message->getStringMessageID());
        mLogger->logDebug(logMessage);
    }

    // Add to Queue
    mMessageQueue.push(message);
}

void Core::MessageNetwork::addSubscriber(const MessageNodeInfo& subscriber)
{
    std::string logMessage("[Adding Subscriber] Subscriber: " + subscriber.nodeName + " Topics: ");
    for(const auto& IDs : subscriber.subscriptions)
    {
        logMessage += messageIDEnumToString(IDs) + " ";

        mSubscriberList.insert(std::make_pair(IDs, subscriber));
    }

    if constexpr (Utility::CAN_LOG)
        mLogger->logDebug(logMessage);
        
}

void Core::MessageNetwork::insertUnsubscriber(const Messages::ID& messageID, const std::string& nodeName)
{
    // Ensure we are not adding duplicate UnSubscribe messages
    bool isDuplicate = false;
    for(auto it = mUnsubscribeList.lower_bound(messageID),
            end = mUnsubscribeList.upper_bound(messageID); it != end; ++it)
    {
        if(it->second == nodeName)
        {
            isDuplicate = true;
            break;
        }
    }

    if(!isDuplicate)
        mUnsubscribeList.insert(std::make_pair(messageID, nodeName));
}

void Core::MessageNetwork::notifySubscribers()
{
    std::string logMessage;
    Messages::ID messageID;

    while( !mMessageQueue.empty())
    {
        messageID = mMessageQueue.front().get()->getMessageID();

        if constexpr (Utility::CAN_LOG)
            logMessage += "[Publishing] Message: " + messageIDEnumToString(messageID) + " Subscriber(s): ";

        for(auto it = mSubscriberList.lower_bound(messageID),
            end = mSubscriberList.upper_bound(messageID); it != end; ++it)
        {
            if constexpr (Utility::CAN_LOG)
                logMessage += it->second.nodeName + " ";

            // Publish Message
            it->second.callback(mMessageQueue.front().get());
        }

        if constexpr (Utility::CAN_LOG)
        {
            mLogger->logDebug(logMessage);
            logMessage.clear();
        }

        mMessageQueue.pop();
    }  

    // Unsubscribe if there are any to unsubscribe to
    if(!mUnsubscribeList.empty())
        unSubscribe();
}

void Core::MessageNetwork::unSubscribe()
{
    std::string logMessage;
    for(const auto& unsubscriber : mUnsubscribeList)
    {
        logMessage += "[Unsubscribing]";

        for(auto it = mSubscriberList.lower_bound(unsubscriber.first),
            end = mSubscriberList.upper_bound(unsubscriber.first); it != end;)
        {
            if(it->second.nodeName == unsubscriber.second)
            {
                if constexpr (Utility::CAN_LOG)
                {
                    logMessage += " Message: " + messageIDEnumToString(unsubscriber.first);
                    logMessage += " Node: " + unsubscriber.second;
                    mLogger->logDebug(logMessage);
                }

                mSubscriberList.erase(it++);
                break;
            }
            else
            {
                ++it;
            }
        }
    }
    mUnsubscribeList.clear();
}

void Core::MessageNetwork::initializeLogger()
{
    // Create and Register
    const std::string outDirectory = Utility::LogRegistry::instance()->getAppOutputDir();

    mLogger = Utility::Factory::createTextFileLogger("MessageNetworkLogger", outDirectory, "MessageNetwork", 
                                                     ".log", Utility::LogLevel::DEBUG);
                                                     
    if constexpr (Utility::CAN_LOG)
        mLogger->logInfo("Logger Initialized");
}