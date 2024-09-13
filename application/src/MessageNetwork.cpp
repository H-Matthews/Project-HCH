#include "MessageNetwork.hpp"
#include "PlayerActionMessage.hpp"
#include "EnemySpawnMessage.hpp"

#include <iostream>

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
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
            auto msg = pam->clone();
            mMessageQueue.push(std::move(msg));

            break;
        }
        case Message::ID::EnemySpawnMessage:
        {
            EnemySpawnMessage* esm = dynamic_cast<EnemySpawnMessage*>( message ); 
            auto msg = esm->clone();
            mMessageQueue.push(std::move(msg));

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

}