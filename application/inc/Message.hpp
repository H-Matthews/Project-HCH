#pragma once

#include "MessageFlag.hpp"

enum class MessageType
{
    Movement = 0,
    Combat
};

struct Message
{
    public:
        Message(MessageFlag messageRecipients, MessageType messageType);

        MessageFlag mMessageRecipients;
        MessageType mMessageType;
};  