#include "MessageNetwork.hpp"
#include "PlayerActionMessage.hpp"
#include "EnemySpawnMessage.hpp"

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

const std::map< Messages::ID, std::function< std::unique_ptr< Message > () > >& MessageNetwork::getMessageRegistry() const
{
    return mMessageRegistry;
}

void MessageNetwork::sendMessage(Message* message)
{
    // Determine Message Type and Send
    PlayerActionMessage* pam = dynamic_cast<PlayerActionMessage*>( message );
    if(pam)
    {
        // Make copy of PlayerActionMessage and send to Queue
        auto msg = std::make_unique< PlayerActionMessage > ( pam );
        mMessageQueue.push(std::move(msg));

        pam = nullptr;
        return;
    }

    EnemySpawnMessage* esm = dynamic_cast<EnemySpawnMessage*>( message );
    if(esm)
    {
        auto msg = std::make_unique< EnemySpawnMessage > ( esm );
        mMessageQueue.push(std::move(msg));

        esm = nullptr;
        return;
    }


    return;
}

void MessageNetwork::addSubscriber(std::pair< std::set< Messages::ID >, MessageNodeInfo > subscriber)
{
    for(const auto& topic : subscriber.first)
    {
        std::pair< Messages::ID, MessageNodeInfo > singleSubscriberEntry(topic, subscriber.second);

        mSubscriberList.insert(singleSubscriberEntry);
    }
}

void MessageNetwork::notifySubscribers()
{
    Messages::ID messageID;

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