#include "core/inc/Messaging/MessageNetwork.hpp"

#include "utility/inc/Logging/Sinks/TextFileSink.hpp"
#include "utility/inc/Logging/Formatters/KeyValueFormatter.hpp"

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
    for(const auto& ID : subscriber.subscriptions)
    {
        std::string messageID = messageIDEnumToString(ID);
        logMessage += messageID + " ";

        if(isDuplicateSubscriber(ID, subscriber.nodeName))
        {
            if constexpr(Utility::CAN_LOG)
                mLogger->logError("Attempted to add duplicate Subscriber. Subscriber: " + subscriber.nodeName + 
                    " MessageID: " + messageID);
        }
        else
        {
            mSubscriberList.insert(std::make_pair(ID, subscriber));
        }
    }

    if constexpr (Utility::CAN_LOG)
        mLogger->logDebug(logMessage);
        
}

bool Core::MessageNetwork::isDuplicateSubscriber(const Messages::ID key, const std::string& node)
{
    bool isDuplicate = false;

    auto rangeIT = mSubscriberList.equal_range(key);
    for(auto keyIT = rangeIT.first; keyIT != rangeIT.second; ++keyIT)
    {
        if(keyIT->second.nodeName == node)
        {
            isDuplicate = true;
            break;
        }
    }

    return isDuplicate;
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

    mLogger = Utility::createTextFileLogger("MessageNetworkLogger", outDirectory, "MessageNetwork", 
                                                     ".log", Utility::LogLevel::DEBUG);
                                                     
    if constexpr (Utility::CAN_LOG)
        mLogger->logInfo("Logger Initialized");
}

void Core::MessageNetwork::shutdownNetwork()
{
    mSubscriberList.clear();
    mUnsubscribeList.clear();

    // This "clears" the Queue
    mMessageQueue = {};
}