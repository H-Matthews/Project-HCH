#include "core/inc/Messaging/MessageNodeInfo.hpp"


Core::MessageNodeInfo::MessageNodeInfo(const std::string& name, std::function<void (Message*) > func) :
    nodeName(name),
    subscriptions(),
    callback(func)
{
}