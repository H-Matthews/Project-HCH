#include "core/inc/Messaging/MessageNetwork.hpp"

#include <iostream>

Core::MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

void Core::MessageNetwork::sendMessage(Message* message)
{
    // Clone and put onto Queue
    std::unique_ptr<Message> clonedMessage = message->clone();
    mMessageQueue.push(std::move(clonedMessage));

}

void Core::MessageNetwork::addSubscriber(const MessageNodeInfo& subscriber)
{
    for(const auto& IDs : subscriber.subscriptions)
    {
        mSubscriberList.insert(std::make_pair(IDs, subscriber));
    }
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
    Messages::ID messageID;

    while( !mMessageQueue.empty())
    {
        messageID = mMessageQueue.front().get()->getMessageID();

        for(auto it = mSubscriberList.lower_bound(messageID),
            end = mSubscriberList.upper_bound(messageID); it != end; ++it)
        {
            // Potentially Dangerous... If the subscriber attempts to use 
            // this object in anyway after popping the message from the queue
            // it will result in a segfault as the Queue owns the memory for messages
            // Not going to clone everytime as that is not necessary, if the message needs to live 
            // after popping from the Queue, the subscriber needs to clone the message
            it->second.callback(mMessageQueue.front().get());
        }
        mMessageQueue.pop();
    }  

    // Unsubscribe if there are any to unsubscribe to
    unSubscribe();
}

void Core::MessageNetwork::unSubscribe()
{
    for(const auto& unsubscriber : mUnsubscribeList)
    {
        for(auto it = mSubscriberList.lower_bound(unsubscriber.first),
            end = mSubscriberList.upper_bound(unsubscriber.first); it != end;)
        {
            if(it->second.nodeName == unsubscriber.second)
            {
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