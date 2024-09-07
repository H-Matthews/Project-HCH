#include "MessageNetwork.hpp"

#include <iostream>

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

void MessageNetwork::sendMessage(Message message)
{
    mMessageQueue.push(message);
}

void MessageNetwork::addSubscriber(std::pair< std::vector< MessageTopic >, MessageSubscriptionInfo > subscriber)
{
    
    for(const auto& topic : subscriber.first)
    {
        std::pair< MessageTopic, MessageSubscriptionInfo > singleSubscriberEntry(topic, subscriber.second);

        mSubscriberList.insert(singleSubscriberEntry);
    }
}

// Note: Potentially dangerous due to the nested loops
// Although it should be fine to use for PlayerRequests due to the small volume of messages needed
void MessageNetwork::notifySubscribers()
{
    std::vector< MessageTopic > topicList;

    while( !mMessageQueue.empty())
    {
        topicList = mMessageQueue.front().getTopicList();

        // Iterate over ALL message topics
        for(const auto& messageTopic : topicList)
        {
            auto iterator = mSubscriberList.find(messageTopic);
            while(iterator != mSubscriberList.end() && iterator->first == messageTopic )
            {
                iterator->second.callback(mMessageQueue.front());
                iterator++;
            }
        }

        mMessageQueue.pop();
    }

}