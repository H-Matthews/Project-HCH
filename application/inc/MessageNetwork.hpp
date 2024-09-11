#pragma once

#include "Message.hpp"
#include "MessageNodeInfo.hpp"

#include <functional>
#include <string>
#include <queue>
#include <map>
#include <memory>
#include <set>

class MessageNetwork
{
    public:
        MessageNetwork();

        template <typename T>
        void registerMessage(std::pair< Message::ID, std::string > messageID);
        
        const std::map< Message::ID, std::function< std::unique_ptr< Message > () > >& getMessageRegistry() const;

        void sendMessage(Message* message);

        void addSubscriber(const MessageNodeInfo& subscriber);
        void notifySubscribers();

    private:
        std::multimap< Message::ID, MessageNodeInfo > mSubscriberList;
        std::queue< std::unique_ptr< Message > > mMessageQueue;
        std::map< Message::ID, std::function<std::unique_ptr< Message >()> > mMessageRegistry;
};

template <typename T>
void MessageNetwork::registerMessage(std::pair< Message::ID, std::string > messageID)
{
    // Stores a Lambda in mMessageRegistry
    mMessageRegistry[messageID.first] = [messageID] ()
    {
        return std::make_unique< T >(messageID);
    };
}