#include "MessageNetwork.hpp"

#include <iostream>

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

void MessageNetwork::sendMessage(Message* message)
{
    // Make a unique_ptr clone and move to mMessageQueue
    auto msg = std::make_unique< Message >( message ); 

    // Programmer WARNING: do NOT attempt to use msg after this call
    // Its ownership has changed
    mMessageQueue.push(std::move(msg));
}

void MessageNetwork::addSubscriber(std::pair< std::vector< MessageTopic >, MessageSubscriptionInfo > subscriber)
{
    
    for(const auto& topic : subscriber.first)
    {
        std::pair< MessageTopic, MessageSubscriptionInfo > singleSubscriberEntry(topic, subscriber.second);

        mSubscriberList.insert(singleSubscriberEntry);
    }
}

void MessageNetwork::notifySubscribers()
{
    std::vector< MessageTopic > topicList;

    while( !mMessageQueue.empty())
    {
        topicList = mMessageQueue.front()->getTopicList();

        // Iterate over ALL message topics
        for(const auto& messageTopic : topicList)
        {
            auto iterator = mSubscriberList.find(messageTopic);
            while(iterator != mSubscriberList.end() && iterator->first == messageTopic )
            {
                // Potentially Dangerous... If the callback function attempts to use 
                // this object in anyway after its initial use... It will be a nullptr
                // Proper way to deal with this is to clone the object
                iterator->second.callback(mMessageQueue.front().get());
                iterator++;
            }
        }

        mMessageQueue.pop();
    }

}