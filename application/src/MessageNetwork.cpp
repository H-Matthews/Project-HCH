#include "MessageNetwork.hpp"

void MessageNetwork::addListener(std::function<void (Message)> messageListener)
{
    listener.push_back(messageListener);
}

void MessageNetwork::addMessage(Message message)
{
    messageQueue.push(message);
}

void MessageNetwork::notifyListeners()
{
    while( !messageQueue.empty())
    {
        for(auto iter = listener.begin(); iter != listener.end(); ++iter)
        {
            (*iter)(messageQueue.front());
        }

        messageQueue.pop();
    }
}