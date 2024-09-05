#pragma once

#include "stdint.h"

enum class MessageTopics
{
    PLAYER = 1 << 0, // 1
    ENEMY  = 1 << 1, // 2
};

class MessageFlag 
{
    public:
        MessageFlag(uint8_t flagValue);

        void setFlag(MessageTopics flag);

        void unsetFlag(MessageTopics flag);

        bool hasFlag(MessageTopics flag);

        uint8_t getFlagValue();
    
    private:
        uint8_t mFlagValue;
};