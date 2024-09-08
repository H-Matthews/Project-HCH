#pragma once

#include "Message.hpp"

//Utility
#include "StringOperations.hpp"

#include <functional>
#include <string>
#include <queue>
#include <map>
#include <memory>
#include <set>

struct MessageNodeInfo
{
    std::string nodeName;
    std::function<void(Message*)> callback;

    MessageNodeInfo(std::string name) { nodeName = name; }
    MessageNodeInfo() {}
};

class MessageNetwork
{
    public:
        MessageNetwork();

        template <typename T>
        void registerMessage(Messages::ID messageID);
        
        std::map< Messages::ID, std::function< std::unique_ptr< Message > () > >& getMessageRegistry();

        void sendMessage(Message* message);

        void addSubscriber(std::pair< std::set< Messages::ID >, MessageNodeInfo > subscriber);
        void notifySubscribers();

    private:
        std::multimap< Messages::ID, MessageNodeInfo > mSubscriberList;
        std::queue< std::unique_ptr< Message > > mMessageQueue;
        std::map< Messages::ID, std::function<std::unique_ptr< Message >()> > mMessageRegistry;
};

template <typename T>
void MessageNetwork::registerMessage(Messages::ID messageID)
{
    const std::string identifierString(Utility::messageEnumToString(messageID));

    // Stores a Lambda in mMessageRegistry
    mMessageRegistry[messageID] = [this, identifierString] ()
    {
        return std::unique_ptr< Message >(new T(*this, identifierString));
    };
}