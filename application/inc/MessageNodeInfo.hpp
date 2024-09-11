#pragma once

#include "Message.hpp"

#include <set>
#include <string>
#include <functional>

struct MessageNodeInfo
{
    std::string nodeName;
    std::set< Message::ID > subscriptions;
    std::set< std::string > stringSubscriberList;
    std::function<void (Message*) > callback;
    MessageNodeInfo(const std::string& name);
};