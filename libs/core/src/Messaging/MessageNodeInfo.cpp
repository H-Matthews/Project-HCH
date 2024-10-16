#include "core/inc/Messaging/MessageNodeInfo.hpp"


Core::MessageNodeInfo::MessageNodeInfo(const std::string& name) :
    nodeName(name),
    subscriptions(),
    callback(nullptr)
{
}