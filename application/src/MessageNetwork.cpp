#include "MessageNetwork.hpp"

#include <iostream>

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
}

void MessageNetwork::addSubscriber(std::pair< MessageTopicFlag, MessageSubscriptionInfo > subscriber)
{
    std::vector< MessageTopic > topicList = subscriber.first.determineTopics();

    for(const auto& topic : topicList)
    {
        std::pair< MessageTopic, MessageSubscriptionInfo > singleSubscriberEntry(topic, subscriber.second);

        mSubscriberList.insert(singleSubscriberEntry);
    }
}

void MessageNetwork::sendMessage(Message message)
{
    mMessageQueue.push(message);
}

// Note: Potentially dangerous due to the nested loops
// Although it should be fine to use for PlayerRequests due to the small volume of messages needed
void MessageNetwork::notifySubscribers()
{
    std::vector< MessageTopic > topicList;
    topicList.reserve(3); // Number of Topics

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

        topicList.clear();
        mMessageQueue.pop();
    }

}