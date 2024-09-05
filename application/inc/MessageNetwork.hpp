#pragma once
#include "Message.hpp"

#include <functional>
#include <vector>
#include <queue>
#include <map>

class Entity;

class MessageNetwork
{
    public:
        void addListener(std::function<void (Message)> messageListener);
        void addMessage(Message message);
        void notifyListeners();

    private:
        std::vector<std::function<void (Message)>> listener;
        std::queue<Message> messageQueue;
};