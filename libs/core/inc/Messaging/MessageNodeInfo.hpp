#pragma once

#include "core/inc/Messaging/Message.hpp"

#include <set>
#include <string>
#include <functional>

namespace Core 
{
    
    struct MessageNodeInfo
    {
        std::string nodeName;
        std::set< Messages::ID > subscriptions;
        std::function<void (Message*) > callback;
        MessageNodeInfo(const std::string& name);
    };

}
