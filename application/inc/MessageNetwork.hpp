#pragma once

#include "Message.hpp"

//Utility
#include "StringOperations.hpp"

#include <functional>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

struct MessageSubscriptionInfo
{
    std::string name;
    std::function<void(Message*)> callback;

    MessageSubscriptionInfo(std::string nodeName) { name = nodeName; }
};

class MessageNetwork
{
    public:
        MessageNetwork();

        template <typename T>
        void registerMessage(Messages::ID messageID);
        
        std::map< Messages::ID, std::function< std::unique_ptr< Message > () > >& getMessageRegistry();

        void sendMessage(Message* message);

        void addSubscriber(std::pair< std::vector< Messages::ID >, MessageSubscriptionInfo > subscriber);
        void notifySubscribers();

    private:
        std::multimap< Messages::ID, MessageSubscriptionInfo > mSubscriberList;
        std::queue< std::unique_ptr< Message > > mMessageQueue;
        std::map< Messages::ID, std::function<std::unique_ptr< Message >()> > mMessageRegistry;
};

template <typename T>
void MessageNetwork::registerMessage(Messages::ID messageID)
{
    const std::string identifierString(Utility::messageEnumToString(messageID));

    // Stores a Lambda in mRegistry
    mMessageRegistry[messageID] = [this, identifierString] ()
    {
        return std::unique_ptr< Message >(new T(*this, identifierString));
    };
}