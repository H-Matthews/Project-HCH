#include "MessageNetwork.hpp"

MessageNetwork::MessageNetwork() :
    mSubscriberList(),
    mMessageQueue()
{
    registerTopic(MessageTopic::PLAYER);
    registerTopic(MessageTopic::ENEMY);
}

void MessageNetwork::addSubscriber(MessageTopicFlag topicFlag, std::function<void (Message)> messageSubscriber)
{
    if(topicFlag.getFlagValue() != 0)
    {
        // Add subscriber to map 
        for(auto& [topic, subscribers] : mSubscriberList)
        {
            if(topicFlag.hasFlag(topic))
                subscribers.push_back(messageSubscriber);
        }
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
    while( !mMessageQueue.empty())
    {
        // Loop through all topics the message is publishing
        for(const auto& topic : mMessageQueue.front().getTopicList())
        {
            // Gets list of subscribers for that topic
            if(mSubscriberList.find(topic) != mSubscriberList.end())
            {
                // Call each subscriber
                for(const auto& subscriber : mSubscriberList[topic])
                {
                    subscriber(mMessageQueue.front());
                }
            }
        }

        mMessageQueue.pop();
    }
}

bool MessageNetwork::registerTopic(MessageTopic topic)
{
    bool successfulInsertion = false;

    // Ensure the topic is not already in the map
    if(mSubscriberList.find(topic) == mSubscriberList.end())
    {
        mSubscriberList[topic];
        successfulInsertion = true;
    }

    return successfulInsertion;
}