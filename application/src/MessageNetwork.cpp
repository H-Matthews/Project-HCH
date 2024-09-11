#include "MessageNetwork.hpp"
#include "PlayerActionMessage.hpp"
#include "EnemySpawnMessage.hpp"

#include <iostream>

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

const std::map< Message::ID, std::function< std::unique_ptr< Message > () > >& MessageNetwork::getMessageRegistry() const
{
    return mMessageRegistry;
}

void MessageNetwork::sendMessage(Message* message)
{
    // Determine Message Type and Send
    switch (message->getMessageID())
    {
        case Message::ID::NONE:
        {
            std::cout << "MessageNetwork::sendMessage(): Message does not have a ID associated with it\n";
            break;
        }
        case Message::ID::PlayerActionMessage:
        {
            PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( message ); 
            auto msg = std::make_unique< PlayerActionMessage > ( pam );
            mMessageQueue.push(std::move(msg));

            pam = nullptr;
            break;
        }
        case Message::ID::EnemySpawnMessage:
        {
            EnemySpawnMessage* esm = dynamic_cast<EnemySpawnMessage*>( message ); 
            auto msg = std::make_unique< EnemySpawnMessage > ( esm );
            mMessageQueue.push(std::move(msg));

            esm = nullptr;
            break;
        }
    }
}

void MessageNetwork::addSubscriber(const MessageNodeInfo& subscriber)
{
    for(const auto& IDs : subscriber.subscriptions)
    {
        mSubscriberList.insert(std::make_pair(IDs, subscriber));
    }
}

void MessageNetwork::notifySubscribers()
{
    Message::ID messageID;

    while( !mMessageQueue.empty())
    {
        messageID = mMessageQueue.front().get()->getMessageID();

        auto iterator = mSubscriberList.find(messageID);
        while(iterator != mSubscriberList.end() && iterator->first == messageID )
        {
            // Potentially Dangerous... If the subscriber attempts to use 
            // this object in anyway after popping the message from the queue... 
            // It will be a nullptr
            // Proper way to deal with this is to clone the object
            iterator->second.callback(mMessageQueue.front().get());
            iterator++;
        }
        
        mMessageQueue.pop();
    }  

}