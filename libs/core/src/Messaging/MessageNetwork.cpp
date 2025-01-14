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
    std::stringstream logStream;
    logStream << "[Sending Message] Sender: " << message->getSenderName() << " MessageID: " << message->getStringMessageID();
    mLogger->logDebug(logStream.str());

    // Add to Queue
    mMessageQueue.push(message);
}

void Core::MessageNetwork::addSubscriber(const MessageNodeInfo& subscriber)
{
    std::stringstream logStream;
    logStream << "[Adding Subscriber] Subscriber: " << subscriber.nodeName << " Topics: ";

    for(const auto& IDs : subscriber.subscriptions)
    {
        logStream << messageIDEnumToString(IDs) << " ";
        mSubscriberList.insert(std::make_pair(IDs, subscriber));
    }

    mLogger->logDebug(logStream.str());
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
    std::stringstream logStream;

    Messages::ID messageID;

    while( !mMessageQueue.empty())
    {
        messageID = mMessageQueue.front().get()->getMessageID();
        logStream << "[Disseminating]";
        logStream << " Message: " << messageIDEnumToString(messageID);
        logStream << " Subscriber(s): ";

        for(auto it = mSubscriberList.lower_bound(messageID),
            end = mSubscriberList.upper_bound(messageID); it != end; ++it)
        {
            logStream << it->second.nodeName << " ";

            // Disseminate Message
            it->second.callback(mMessageQueue.front().get());
        }

        mLogger->logDebug(logStream.str());
        logStream.str("");

        mMessageQueue.pop();
    }  

    // Unsubscribe if there are any to unsubscribe to
    if(!mUnsubscribeList.empty())
        unSubscribe();
}

void Core::MessageNetwork::unSubscribe()
{
    std::stringstream logStream;
    for(const auto& unsubscriber : mUnsubscribeList)
    {
        logStream.str("");
        logStream << "[Unsubscribing]";

        for(auto it = mSubscriberList.lower_bound(unsubscriber.first),
            end = mSubscriberList.upper_bound(unsubscriber.first); it != end;)
        {
            if(it->second.nodeName == unsubscriber.second)
            {
                logStream << " Message: " << messageIDEnumToString(unsubscriber.first);
                logStream << " Node: " << unsubscriber.second;
                mLogger->logDebug(logStream.str());

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
    mLogger->logInfo("Logger Initialized");
}