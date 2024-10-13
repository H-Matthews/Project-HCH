#include "core/inc/Messaging/MessageNodeInfo.hpp"


MessageNodeInfo::MessageNodeInfo(const std::string& name) :
    nodeName(name),
    subscriptions(),
    callback(nullptr)
{
}